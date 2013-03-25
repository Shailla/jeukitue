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

#include "util/GenRef.h"
#include "util/types/Data.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/Donnee.h"
#include "data/Valeur.h"
#include "data/Distant.h"

class Branche : public Donnee {
	Branche* _parent;
	int _brancheId;
	int _brancheTmpId;
	std::string _brancheName;

	std::map<int, Branche*> _subBranches;
	std::map<int, Valeur*> _valeurs;

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;
public:
	Branche(Branche* parent, int brancheId, const std::string& brancheName, int revision, int tmpId);
	virtual ~Branche();

	Branche* getSubBrancheByDistantTmpId(Distant* distant, int brancheTmpId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByTmpId(int brancheTmpId) const;
	Branche* getSubBranche(int brancheId) const;
	std::map<int, Branche*>& getSubBranches();

	std::map<int, Valeur*>& getValeurs();
	Valeur* getValeur(int valeurId);

	std::string getBrancheName() const;
	int getBrancheId() const;
	int getBrancheTmpId() const;
	std::vector<int> getParentBrancheId(void) const;
	void getBrancheFullId(std::vector<int>& id) const;
	std::vector<int> getBrancheFullId() const;

	/** Attribue son identifiant d�finitf � une branche temporaire */
	Branche* acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	/** Cr�e une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName);

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(const std::string& brancheName);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant temporaire */
	Valeur* createValeurIntForClient(const std::string& valeurName, int valeur);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	Valeur* createValeurIntForServeur(const std::string& valeurName, int valeur);

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Valeur* addValeurInt(int valeurId, const std::string& valeurName, int valeurRevision, JktUtils::Data* valeur);
};

#endif /* BRANCHE_H_ */
