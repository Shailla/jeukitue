/*
 * ClientDataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENTDATATREE_H_
#define CLIENTDATATREE_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

#include "util/types/Data.h"

#include "data/DataTree.h"

class ClientDataTree : public DataTree {
	std::string _clientName;
	Distant* _serveur;

	void initDistantBranche(Distant* distant, Branche* branche);
	void addServeurMarqueur(Donnee* donnee);
public:
	ClientDataTree(Distant* server, const std::string& clientName);
	virtual ~ClientDataTree();

	const string& getClientName() const;
	Branche* getBrancheByTmpId(const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);

	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName, int revision);
	Valeur* createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, int revision, const JktUtils::Data* valeur);
	Valeur* updateValeur(const std::vector<int>& brancheId, int valeurId, const JktUtils::Data* value);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	Distant* getDistantServer() const;

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToServer(void);
	void receiveChangementsFromServer();
	void sendChangementsToServer(vector<Changement*>& changements);
};

#endif /* CLIENTDATATREE_H_ */
