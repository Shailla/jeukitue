/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "util/types/IntData.h"
#include "data/ValeurInt.h"

#include "data/Branche.h"

Branche::Branche(Branche* parent, int brancheId, const string& brancheName, int revision, int tmpId) : Donnee(revision) {
	_parent = parent;
	_brancheId = brancheId;
	_brancheName = brancheName;
	_tmpId = tmpId;
}

Branche::~Branche() {
}

Branche* Branche::getSubBranche(int brancheId) const {
	Branche* branche = NULL;

	try {
		branche = _subBranches.at(brancheId);
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* Branche::createSubBrancheForClient(const string& brancheName) {
	// Alloue une r�f�rence pour la nouvelle branche
	int tmpRef = _brancheTmpRefGenerator.genRef();		// On d�marre � 1

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(this, -1, brancheName, 0, tmpRef);
	_subBranches[-tmpRef] = newBranche;		// Moins 1 pour �viter les collisions avec les ID non temporaires

	return newBranche;
}


Branche* Branche::createSubBrancheForServer(const string& brancheName) {
	// Alloue une r�f�rence pour la nouvelle branche
	int ref = _brancheRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(this, ref, brancheName, 0, -1);
	_subBranches[ref] = newBranche;

	return newBranche;
}

Branche* Branche::addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision) {
	if(_subBranches.find(brancheId) != _subBranches.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " La branche existe deja";
	}

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(this, brancheId, brancheName, brancheRevision, -1);
	_subBranches[brancheId] = newBranche;

	return newBranche;
}

Valeur* Branche::createValeurIntForClient(const string& valeurName, int valeur) {
	// Alloue une r�f�rence pour la nouvelle branche
	int tmpRef = _valeurTmpRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Valeur* newValeur = new ValeurInt(this, -1, valeurName, valeur, tmpRef);
	_valeurs[-tmpRef] = newValeur;

	return newValeur;
}

Valeur* Branche::createValeurIntForServeur(const string& valeurName, int valeur) {
	// Alloue une r�f�rence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Valeur* newValeur = new ValeurInt(this, ref, valeurName, valeur, -1);
	_valeurs[ref] = newValeur;

	return newValeur;
}

Valeur* Branche::addValeurInt(int valeurId, const string& valeurName, int valeurRevision, JktUtils::Data* valeur) {
	Valeur* newValeur = NULL;

	if(_valeurs.find(valeurId) != _valeurs.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " La valeur existe deja";
	}

	// Cr�e la nouvelle valeur
	if(JktUtils::IntData* intData = dynamic_cast<JktUtils::IntData*>(valeur)) {
		newValeur = new ValeurInt(this, valeurId, valeurName, intData->getValue(), -1);
		_valeurs[valeurId] = newValeur;
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
	}

	return newValeur;
}

map<int, Branche*>& Branche::getSubBranches() {
	return _subBranches;
}

map<int, Valeur*>& Branche::getValeurs() {
	return _valeurs;
}

Valeur* Branche::getValeur(int valeurId) {
	return _valeurs.at(valeurId);
}

string Branche::getBrancheName() const {
	return _brancheName;
}

int Branche::getBrancheId() const {
	return _brancheId;
}


vector<int> Branche::getParentBrancheId(void) const {
	vector<int> parentBrancheId;

	if(_parent) {
		parentBrancheId = _parent->getBrancheFullId();
	}
	else {
		// Do nothing, return empty id
	}

	return parentBrancheId;
}

void Branche::getBrancheFullId(vector<int>& id) const {
	if(_parent) {
		_parent->getBrancheFullId(id);
		id.push_back(_brancheId);
	}
}

vector<int> Branche::getBrancheFullId() const {
	vector<int> id;
	getBrancheFullId(id);

	return id;
}
