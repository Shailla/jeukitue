/*
 * ServeurDataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef SERVEURDATATREE_H_
#define SERVEURDATATREE_H_

#include <string>
#include <map>
#include <vector>

#include "data/DataTree.h"

class ServeurDataTree : public DataTree {
	std::vector<Distant*> _clients;

	Donnee* addMarqueurFromDistant(Distant* distant, Donnee* valeur, int donneeClientTmpId);
	void initDistantBranche(Distant* distant, Branche* branche);

public:
	ServeurDataTree();
	virtual ~ServeurDataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName);
	Valeur* createValeurInt(const std::vector<int>& parentBrancheId, const std::string& valeurName, int valeur);


	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* getBrancheByDistantTmpId(Distant* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);

	Branche* addBrancheFromDistant(const vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, Distant* distant);
	Valeur* addValeurIntFromDistant(const vector<int>& parentBrancheId, const std::string& valeurName, int valeurTmpId, int valeur, Distant* distant);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	Distant* addDistant(const std::string& distantName);
	std::vector<Distant*>& getDistants();


	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToClients(void);
	void receiveChangementsFromClient();
};

#endif /* SERVEURDATATREE_H_ */
