/*
 * Valeur.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEUR_H_
#define VALEUR_H_

#include <string>

using namespace std;

#include "util/GenRef.h"

class Valeur {
	int _valeurId;

	JktUtils::CGenRef _referenceGenerator;
public:
	Valeur();
	virtual ~Valeur();

	int getValeurId() const;
};

#endif /* VALEUR_H_ */
