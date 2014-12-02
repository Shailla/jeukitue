#ifndef __JKT__PORTE_H
#define __JKT__PORTE_H

#include "spatial/Mouve.h"

#include "data/ValeurFloat.h"

class CGame;
class CPlayer;

namespace JktMoteur
{

class CMap;
class CIstreamMap;

class Dirigeable : public CMouve {
	CMap* _map;

	// Vitesse de l'objet dans l'arbre de donn�es
	ValeurFloat* _vitesseX;
	ValeurFloat* _vitesseY;
	ValeurFloat* _vitesseZ;

	// Position de l'objet dans l'arbre de donn�es
	ValeurFloat* _positionX;
	ValeurFloat* _positionY;
	ValeurFloat* _positionZ;

public:
	Dirigeable(CMap *map);
	~Dirigeable();

	void Init();					// Initialisation de l'objet g�om�trique
	void Affiche();					// Fonction d'affichage de l'objet g�om�trique
	void Refresh(CGame *game);		// Rafraichissement des donn�es, position, vitesse, ... de l'objet
};

}	// JktMoteur

#endif

