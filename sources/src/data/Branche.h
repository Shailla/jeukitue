/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef BRANCHE_H_
#define BRANCHE_H_

#include <string>
#include <map>

using namespace std;

#include "util/types/Data.h"
#include "data/Donnee.h"
#include "data/Valeur.h"
#include "util/GenRef.h"

class Branche : public Donnee {
	Branche* _parent;
	int _brancheId;
	string _brancheName;

	map<int, Branche*> _subBranches;
	map<int, Valeur*> _valeurs;

	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
public:
	Branche(Branche* parent, int brancheId, const string& brancheName);
	Branche(Branche* parent, int brancheId, const string& brancheName, int revision);
	virtual ~Branche();

	Branche* getSubBranche(int brancheId) const;
	map<int, Branche*>& getSubBranches();
	map<int, Valeur*>& getValeurs();
	Valeur* getValeur(int valeurId);
	string getBrancheName() const;
	int getBrancheId() const;
	vector<int> getParentBrancheId(void) const;
	void getBrancheFullId(vector<int>& id) const;
	vector<int> getBrancheFullId() const;

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBranche(const string& brancheName);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	Valeur* createValeurInt(const string& valeurName, int valeur);

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	void addValeurInt(int valeurId, const string& valeurName, int valeurRevision, int valeur);
	void addValeurInt(int valeurId, const string& valeurName, int valeurRevision, JktUtils::Data* valeur);
};

#endif /* BRANCHE_H_ */
