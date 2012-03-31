
#ifndef __JKT__PARTICULE_H
#define __JKT__PARTICULE_H

#include "util/V3D.h"

using namespace JktUtils;

namespace JktMoteur
{

class CParticule
{
public:
		// Param�tres
	CV3D position;
	CV3D vitesse;
	CV3D acceleration;
	float masse;					// Masse de la particule
	unsigned int date2naissance;	// Instant de cr�ation de la particule
	unsigned int duree2vie;			// Dur�e de vie de la particule
	bool visible;					// Indique si la particule est visible ou non
	unsigned int number;	// DEBUG
		// Constructeurs
	CParticule();
	CParticule( CV3D pos, CV3D vit, float m, unsigned int duree );

		// Fonctions
	void Calcule();		// Calcule param�tres (position,...)
	bool Vie( unsigned int temps );			// V�rifie si la particule est encore en vie
	void Reset( CV3D pos, CV3D vit );			// R�initialise la particule � une nouvelle position
};

}	// JktMoteur

#endif
