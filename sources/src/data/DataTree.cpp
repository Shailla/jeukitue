/*
 * DataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <vector>

using namespace std;

#include "data/MarqueurBrancheClient.h"
#include "data/MarqueurValeurClient.h"
#include "data/exception/NotExistingBrancheException.h"

#include "data/DataTree.h"

DataTree::DataTree() : _root(0, "root") {
}

DataTree::~DataTree() {
}

Branche* DataTree::getBranche(vector<int> brancheId) throw(NotExistingBrancheException) {
	vector<int>::iterator iter;

	Branche* branche = &_root;

	cout << "getBranche() : " << endl;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		cout << *iter << " => ";
		branche = branche->getSubBranche(*iter);

		if(branche == NULL) {
			throw NotExistingBrancheException();
		}
	}

	return branche;
}

Branche& DataTree::getRoot() {
	return _root;
}

Branche* DataTree::addBranche(vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheForClient(parentBrancheId, brancheName, 0, NULL);
}

Branche* DataTree::addBrancheForClient(vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Branche* branche = parentBranche->createSubBranche(brancheName);

	vector<Client>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client& cl = *clIter;

		MarqueurBrancheClient* marqueur = new MarqueurBrancheClient(branche);

		if(&cl == client) {
			marqueur->setTemporaryId(brancheClientTmpId);
		}

		client->addMarqueur(*marqueur);
	}

	return branche;
}

Valeur* DataTree::addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur) {
	return addValeurIntForClient(parentBrancheId, valeurName, valeur, 0, NULL);
}

Valeur* DataTree::addValeurIntForClient(vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int valeur, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Valeur* val = parentBranche->createValeurInt(valeurName, valeur);

	vector<Client>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client& cl = *clIter;

		MarqueurValeurClient* marqueur = new MarqueurValeurClient(val);

		if(&cl == client) {
			marqueur->setTemporaryId(valeurClientTmpId);
		}

		client->addMarqueur(*marqueur);
	}

	return val;
}




void DataTree::diffuseChangements(void) {
	vector<Client>::iterator clientIter;

	for(clientIter = _clients.begin() ; clientIter != _clients.end() ; clientIter++) {
		Client& client = *clientIter;
		vector<MarqueurClient>& marqueurs = client.getMarqueurs();

		vector<MarqueurClient>::iterator marqIter;

		for(marqIter = marqueurs.begin() ; marqIter != marqueurs.end() ; marqIter++) {
			MarqueurClient& marqueur = *marqIter;

			if(!marqueur.isUpToDate()) {
				//collecte(marqueur);
			}
		}
	}
}
