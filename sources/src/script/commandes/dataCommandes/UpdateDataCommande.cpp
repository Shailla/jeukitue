/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "data/ValeurInt.h"
#include "data/DataTree.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

using namespace JktUtils;

extern DataTree serveurDataTree;

UpdateDataCommande::UpdateDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void UpdateDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "valeur") {
		// Nouvelle valeur de la valeur
		string valeurStr = StringUtils::findAndEraseFirstWord(ligne);

		// Coordonn�es de la branche sur laquelle la valeur doit �tre ajout�e
		vector<int> valeurId = getIntParameters(ligne);

		Valeur* vl = serveurDataTree.getValeur(valeurId);
		ValeurInt* vlInt = dynamic_cast<ValeurInt*>(vl);

		if(vlInt != 0) {
			int valeur = getIntParameter(valeurStr);
			vlInt->updateValeur(valeur);
		}
		else {
			printErrLn("Type de valeur non-pris en compte", userOutput);
		}
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
}

string UpdateDataCommande::getHelp() const {
	return "data update valeur <newValeur> <valeurId> : Valorise la donn�e de type valeur identifi�e par <valeurId> avec la valeur <newValeur>.\
\n<newValeur> depend du type de valeur qui est detecte automatiquement (entier, chaine de caracteres, ...)"\
"\nExemple : data update valeur 88 0 3 2";
}
