/*
 * Valeur.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEURINT_H_
#define VALEURINT_H_

#include <string>

using namespace std;

#include "util/GenRef.h"
#include "data/Valeur.h"

class ValeurInt : public Valeur {
	int _valeur;
public:
	ValeurInt(Branche* parent, int valeurId, const string& valeurName, int valeur, int tmpRef);
	virtual ~ValeurInt();

	int getValeur() const;
	void setValeur(int revision, int valeur);
	void updateValeur(int valeur);
	void setValeur(int revision, JktUtils::Data* data);
};

#endif /* VALEUR_H_ */
