
#include <sstream>

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "data/Valeur.h"
#include "data/ValeurInt.h"

#include "menu/DataTreeView.h"

DataTreeView::DataTreeView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Arbre de la MAP courante");

	_treeServeur = AG_TreetblNew(m_window, AG_TREETBL_EXPAND, NULL, NULL);
	_treeClient1 = AG_TreetblNew(m_window, AG_TREETBL_EXPAND, NULL, NULL);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

	AG_Button* _buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 100, 100);
    hide();
}

DataTreeView::~DataTreeView(void)
{
}

void DataTreeView::show(void) {
	AG_TreetblClearRows(_treeServeur);

	// Arbre des donn�es du serveur
	AG_TreetblAddCol(_treeServeur, 0, "<XXXXXXXXXXXXXXXXXXXXX>", "");
	AG_TreetblAddCol(_treeServeur, 1, "<XXXXXXXXXXXXXXXXXXXXX>", "");
	AG_TreetblAddCol(_treeServeur, 2, "<XXXXXXXXXXXXXXXXXXXXX>", "");
	AG_TreetblAddCol(_treeServeur, 3, "<XXXXXXXXXXXXXXXXXXXXX>", "");
	AG_TreetblAddCol(_treeServeur, 4, "<XXXXXXXXXXXXXXXXXXXXX>", "");

	Branche& root = dataTreeServeur.getRoot();
	drawBranche(&root, _treeServeur, 0);

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

string getValueString(ValeurInt* valeur) {
	ostringstream txt;
	txt << "Valeur " << valeur->getValeurId() << "= '" << valeur->getInt();
	return txt.str();
}

void DataTreeView::drawBranche(Branche* branche, AG_Treetbl* tree, int colId) {
	// T�te de la branche
	ostringstream tete;
	tete << "Branche " << branche->getBrancheId();
	AG_TreetblAddRow(tree, NULL, colId, "%s", tete.str().c_str());

	// Valeurs filles
	{
		map<int, Valeur*> subBranches = branche->getValeurs();
		map<int, Valeur*>::iterator itVa;

		for(itVa = subBranches.begin() ; itVa != subBranches.end() ; itVa++) {
			Valeur* valeur = itVa->second;
			string txt = getValueString(valeur);
			AG_TreetblAddRow(tree, NULL, colId, "%s", txt.c_str());
		}
	}

	// Branches filles
	{
		map<int, Branche*> subBranches = branche->getSubBranches();
		map<int, Branche*>::iterator itBr;

		for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
			drawBranche(itBr->second, _treeServeur, colId + 1);
		}
	}
}

