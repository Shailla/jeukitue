/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef BRANCHE_H_
#define BRANCHE_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "util/GenRef.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/exception/AlreadyExistingBrancheException.h"
#include "data/Donnee.h"
#include "data/Valeur.h"
#include "data/DistantTreeProxy.h"

class Branche : public Donnee {
	Branche* _parent;
	int _brancheId;
	int _brancheTmpId;
	std::string _brancheName;

	std::vector<Branche*> _subBranches;
	std::map<int, Branche*> _subBranchesById;
	std::map<int, Branche*> _subBranchesByTmpId;
	std::map<string, Branche*> _subBranchesByName;

	std::vector<Valeur*> _valeurs;
	std::map<int, Valeur*> _valeursById;
	std::map<int, Valeur*> _valeursByTmpId;

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;

public:
	Branche(Branche* parent, int brancheId, const std::string& brancheName, int revision, int tmpId);
	virtual ~Branche();

	Branche* getSubBrancheByName(const string& brancheName) const;
	Branche* getSubBrancheByIdOrTmpId(int brancheId) const;
	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	std::vector<Branche*>& getSubBranches();

	void setBrancheId(int brancheId);

	std::vector<Valeur*>& getValeurs();
	Valeur* getValeurByIdOrTmpId(int valeurId) const;
	Valeur* getValeurByIdOrDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);

	std::string getBrancheName() const;
	int getBrancheId() const;
	int getBrancheTmpId() const;
	std::vector<int> getParentBrancheId(void) const;
	std::vector<int> getParentBrancheIdOrTmpId(void) const;
	void getBrancheFullId(std::vector<int>& id) const;
	void getBrancheFullIdOrTmpId(std::vector<int>& id) const;
	std::vector<int> getBrancheFullId() const;
	std::vector<int> getBrancheFullIdOrTmpId() const;

	/** Attribue son identifiant d�finitf � une branche temporaire */
	Branche* acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	Valeur* acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException);

	/** Cr�e une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName, int revision);

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(const std::string& brancheName, int revision) throw(AlreadyExistingBrancheException);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	Valeur* createValeurForServeur(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	const Valeur* addValeur(int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur);

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * 2 arbres synchronis�s et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 */
	std::string print(int indentation, bool details);

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * Si details=false seules les informations partag�es sont affich�es, donc 2 arbres synchronis�s
	 * et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 * Si details=true alors plus de caract�risques sont affich�es.
	 */
	void print(std::ostringstream& out, bool details, int indentation);

	static bool highestId(const Branche* left, const Branche* right);
};

#endif /* BRANCHE_H_ */
