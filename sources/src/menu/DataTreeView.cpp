#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ClientDataTree.h"
#include "data/MarqueurDistant.h"
#include "data/Interlocutor.h"

#include "menu/DataTreeView.h"

DataTreeView::DataTreeView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Serveur");

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	AG_Expand(box);

	_serveurDetails = drawWidgets(m_window, box, &serveurDataTree);

    AG_SeparatorNewHoriz(m_window);


	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(boxButtons);

    // Bouton retour
	AG_ButtonNewFn(boxButtons, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);

    // Bouton rafra�chir
    AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p", _serveurDetails);

    // Bouton rafra�chir
    AG_ButtonNewFn(boxButtons, 0, "Ouvrir les fenetres clients", openClientsWindows, "%p", this);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
    hide();
}

DataTreeView::~DataTreeView(void) {
}

DataTreeDetails* DataTreeView::drawWidgets(AG_Window* window, AG_Box* box, DataTree* tree) {
	AG_Box* boxes[2];
	boxes[0] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	boxes[1] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);

	// Liste des donn�es
	AG_Tlist* dataList = AG_TlistNew(boxes[0], AG_TLIST_EXPAND | AG_TLIST_TREE);


	// Tableau des clients
	AG_Table* clientsTable = AG_TableNew(boxes[1], AG_TABLE_EXPAND);
	AG_TableAddCol(clientsTable, "Nom", "<XXXXXXXXXXXX>", NULL);
	AG_TableAddCol(clientsTable, "TmpId", "<XXXXXXXXXXXX>", NULL);
	AG_TableAddCol(clientsTable, "Sent rev.", "<XXXXXXXXXXXX>", NULL);
	AG_TableAddCol(clientsTable, "Conf. rev.", "<XXXXXXXXXXXX>", NULL);
	AG_Expand(clientsTable);

	DataTreeDetails* details = new DataTreeDetails(window, dataList, clientsTable, tree);
	AG_TlistSetChangedFn(dataList, selectionChanged, "%p", details);

	return details;
}

void DataTreeView::selectionChanged(AG_Event* event) {
	DataTreeDetails* details = (DataTreeDetails*)AG_PTR(1);
	refreshClientTable(details);
}

void DataTreeView::openClientsWindows(AG_Event* event) {
	DataTreeView* This = (DataTreeView*)AG_PTR(1);

	map<ClientDataTree*, Interlocutor*>::iterator it;

	for(it = dataRouter.begin() ; it != dataRouter.end() ; it++) {
		ClientDataTree* tree = it->first;
		DataTreeDetails* details = NULL;

		try {
			details = This->_clientDataTrees.at(tree);
		}
		catch(out_of_range& exception) {
			details = NULL;
		}

		if(!details) {
			// Cr�e la fen�tre du client si elle n'existe pas encore
			AG_Window* window = AG_WindowNew(0);
			AG_WindowSetCaption(window, tree->getClientName().c_str());

			AG_Box* box = AG_BoxNewHoriz(window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
			AG_Expand(box);

			AG_SeparatorNewHoriz(window);

			details = drawWidgets(window, box, tree);
			This->_clientDataTrees[tree] = details;

			// Boutons
			AG_Box* boxButtons = AG_BoxNewHoriz(window, 0);
			AG_ExpandHoriz(boxButtons);

			// Bouton rafra�chir
			AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p", details);

			refreshClientTable(details);
			refreshServeur(details);

			AG_WindowSetGeometryAlignedPct(window, AG_WINDOW_TC, 100, 50);
			AG_WindowShow(window);
		}
		else {
			// Rafraichit et affiche la fen�tre si elle existe d�j�
			refreshClientTable(details);
			refreshServeur(details);
			AG_WindowShow(details->getWindow());
		}
	}
}

void DataTreeView::refreshClientTable(DataTreeDetails* details) {
	Donnee* selectedDonnee = (Donnee*)AG_TlistSelectedItemPtr(details->getDataList());


	/* **************************************
	 * Mise � jour du tableau des clients
	 * *************************************/

	AG_TableBegin(details->getClientsTable());

	vector<DistantTreeProxy*> distantsToShow;

	if(ServeurDataTree* serveur = dynamic_cast<ServeurDataTree*>(details->getDataTree())) {
		distantsToShow = serveur->getDistants();
	}
	else if(ClientDataTree* client = dynamic_cast<ClientDataTree*>(details->getDataTree())) {
		distantsToShow.push_back(client->getDistantServer());
	}

	vector<DistantTreeProxy*>::iterator iter;

	for(iter = distantsToShow.begin() ; iter != distantsToShow.end() ; iter++) {
		DistantTreeProxy* client = *iter;
		Interlocutor* interlocutor = client->getInterlocutor();

		if(selectedDonnee) {
			MarqueurDistant* marqueur = client->getMarqueur(selectedDonnee);

			if(marqueur) {
				AG_TableAddRow(details->getClientsTable(), "%s:%d:%d:%d", interlocutor->getName().c_str(), marqueur->getTemporaryId(), marqueur->getSentRevision(), marqueur->getConfirmedRevision());
			}
			else {
				AG_TableAddRow(details->getClientsTable(), "%s:%s:%s:%s", interlocutor->getName().c_str(), "<Pas de marqueur>", "<Bug>", "<Bug>");
			}
		}
		else {
			AG_TableAddRow(details->getClientsTable(), "%s:%s:%s:%s", interlocutor->getName().c_str(), "-", "-", "-");
		}
	}

	AG_TableEnd(details->getClientsTable());
}

void DataTreeView::refresh(AG_Event* event) {
	DataTreeDetails* details = (DataTreeDetails*)AG_PTR(1);

	refreshClientTable(details);
	refreshServeur(details);
}

void DataTreeView::refreshServeur(DataTreeDetails* details) {
	/* ************************************
	 * Mise � jour de l'abre des donn�es
	 * ***********************************/

	AG_TlistBegin(details->getDataList());

	// Root
	Branche* root = &details->getDataTree()->getRoot();
	AG_TlistItem* item = AG_TlistAddPtr(details->getDataList(), NULL, "Root", root);
	int depth = 0;
	item->depth = depth;
	drawBranche(details, root, depth+1);

	AG_TlistEnd(details->getDataList());


	// Rafraichissement de la page
	AG_WindowUpdate(details->getWindow());
}

void DataTreeView::show(void) {
	refreshClientTable(_serveurDetails);
	refreshServeur(_serveurDetails);

	View::show();
}

void DataTreeView::drawBranche(DataTreeDetails* details, Branche* branche, int depth) {
	// Valeurs filles
	{
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator itVa;

		for(itVa = valeurs.begin() ; itVa != valeurs.end() ; itVa++) {
			Valeur* valeur = itVa->second;

			ostringstream txt;

			txt << "Valeur[" << valeur->getValeurId() << " r" << valeur->getRevision() << " '" << valeur->getValeurName() << "'] : '" << valeur->toString() << "'";

			AG_TlistItem* item = AG_TlistAddPtr(details->getDataList(), NULL, txt.str().c_str(), valeur);
			item->depth = depth+1;
		}
	}

	// Branches filles
	{
		map<int, Branche*>& subBranches = branche->getSubBranches();
		map<int, Branche*>::const_iterator itBr;

		for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
			Branche* subBranche = itBr->second;

			ostringstream tete;
			tete << "Branche[" << subBranche->getBrancheId() << " r" << subBranche->getRevision() << " '" << subBranche->getBrancheName()  << "']";

			AG_TlistItem* item = AG_TlistAddPtr(details->getDataList(), NULL, tete.str().c_str(), subBranche);
			item->depth = depth;

			drawBranche(details, subBranche, depth+1);
		}
	}
}

