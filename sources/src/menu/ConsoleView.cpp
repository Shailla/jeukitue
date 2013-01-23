#include <vector>
#include <iostream>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "reseau/SPA.h"

#include "menu/ConsoleView.h"

extern float delta;
extern Uint32 ecart,ecartTimer,ecartDisplay;

ConsoleView::ConsoleView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_PLAIN|AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);

	AG_Notebook* book = AG_NotebookNew(m_window, 0);
	AG_Expand(book);

	/******************************
	 *	ONGLET PRINCIPAL
	 ******************************/

	AG_NotebookTab* tabMain = AG_NotebookAddTab(book, "Principal", AG_BOX_VERT);
	_console = AG_ConsoleNew(tabMain, AG_CONSOLE_EXPAND);

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HFILL);
	_commande = AG_TextboxNew(box, AG_TEXTBOX_STATIC|AG_TEXTBOX_HFILL, "Commande : ");
	AG_WidgetFocus(_commande);
	AG_WidgetEnable(_commande);
	AG_SetEvent(_commande, "textbox-return", controllerCallback, "%i", Controller::ConsoleUserExecuteAction); // L'appui sur ENTER est interpr�t�

	AG_ButtonNewFn(box, 0, "Ok", controllerCallback, "%i", Controller::ConsoleUserExecuteAction);


	/******************************
		ONGLET DES SCORES
	******************************/
	AG_NotebookTab* tabScores = AG_NotebookAddTab(book, "Scores", AG_BOX_VERT);
	AG_LabelNew(tabScores, 0, "Scores 1");
	AG_LabelNew(tabScores, 0, "Scores 2");
	AG_LabelNew(tabScores, 0, "Scores 3");


	/******************************
		ONGLET DES INFO
	******************************/
	AG_NotebookTab* tabInfo = AG_NotebookAddTab(book, "Info", AG_BOX_VERT);
	AG_Notebook* subbookInfo = AG_NotebookNew(tabInfo, 0);

	/******************************
		Sous-onglet Info / Partie
	******************************/
	AG_NotebookTab* subtabPartie = AG_NotebookAddTab(subbookInfo, "Partie", AG_BOX_VERT);
	AG_Scrollview* scrollInfo = AG_ScrollviewNew(subtabPartie, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

	// Nom de la MAP ouverte dans la partie en cours
	_mapOuverteLabel = AG_LabelNew(scrollInfo, 0, "Map ouverte : ");

	// Distance parcourue par le tir laser
	AG_LabelNew(scrollInfo, 0, "Distance laser : %f", &delta);
	AG_LabelNew(scrollInfo, 0, "Ecarts : total=%i ms, timer=%i ms, display=%i ms", &ecart, &ecartTimer, &ecartDisplay);


	/******************************
		Sous-onglet Info / Son
	******************************/
	/*AG_NotebookTab* subtabSon = */AG_NotebookAddTab(subbookInfo, "Son", AG_BOX_VERT);

	/******************************
		Sous-onglet Info / R�seau
	******************************/
	AG_NotebookTab* subtabReseau = AG_NotebookAddTab(subbookInfo, "R�seau", AG_BOX_VERT);
	AG_Scrollview* scrollReseau = AG_ScrollviewNew(subtabReseau, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

	// D�bit r�seau en �mission
	AG_LabelNew(scrollReseau, 0, "Debit en emission : %f ko/s (%f octets)", &JktNet::CSPA::m_fDebitEm, &JktNet::CSPA::m_fTailleEm);

	// D�bit r�seau en r�ception
	AG_LabelNew(scrollReseau, 0, "Debit en reception : %f ko/s (%f octets)", &JktNet::CSPA::m_fDebitRec, &JktNet::CSPA::m_fTailleRec);

	// Disposition de la fen�tre	
	AG_WidgetUpdate(book);
	AG_NotebookSelectTab (book, tabMain);
	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_BL, 40, 30);
	AG_WindowShow(m_window);

    hide();
}

ConsoleView::~ConsoleView(void) {
}

void ConsoleView::executeCommande() {
	char* commande = AG_TextboxDupString(_commande);

	if(!JktUtils::StringUtils::isBlank(commande)) {
		println(commande);
		AG_TextboxSetString(_commande, "");
	}

	free(commande);
}

void ConsoleView::println(const char* texte) {
	AG_ConsoleMsgS(_console, texte);
}

void ConsoleView::setMapOuverte(const std::string& mapName) {
	std::string label;
	label.append("Map ouverte : '").append(mapName).append("'");
	AG_LabelText(_mapOuverteLabel, label.c_str() );
}
