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
#include "data/AbstractBranche.h"
#include "data/Valeur.h"
#include "data/DistantTreeProxy.h"

class Branche : public AbstractBranche {
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
	Branche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int tmpId);
	virtual ~Branche();

	/* ****************************************************** */
	// Fonctions compatibles avec les branches priv�es
	/* ****************************************************** */

	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);

	/** Cr�e une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName, int revision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/* ****************************************************** */
	// Fonctions non-compatibles avec les branches priv�es
	/* ****************************************************** */

	virtual Branche* getSubBrancheByName(const string& brancheName) const;
	virtual Branche* getSubBrancheByIdOrTmpId(int brancheId) const;
	virtual std::vector<Branche*>& getSubBranches();

	virtual std::vector<Valeur*>& getValeurs();
	virtual Valeur* getValeurByIdOrTmpId(int valeurId) const;

	/** Attribue son identifiant d�finitf � une branche temporaire */
	virtual Branche* acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	virtual Valeur* acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException);

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	virtual Branche* createSubBrancheForServer(const std::string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	virtual Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	virtual Valeur* createValeurForServeur(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	virtual const Valeur* addValeur(int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur);

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * Si details=false seules les informations partag�es sont affich�es, donc 2 arbres synchronis�s
	 * et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 * Si details=true alors plus de caract�risques sont affich�es.
	 */
	virtual void print(std::ostringstream& out, bool details, int indentation);

	static bool highestId(const Branche* left, const Branche* right);
};

#endif /* BRANCHE_H_ */
