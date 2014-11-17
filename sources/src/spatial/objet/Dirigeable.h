#ifndef __JKT__PORTE_H
#define __JKT__PORTE_H

#include "spatial/Mouve.h"

#include "util/V3D.h"

class CGame;
class CPlayer;

namespace JktMoteur
{

class CMap;
class CIstreamMap;

class Dirigeable : public CMouve
{
	JktUtils::CV3D _position;		// Position de l'objet

public:
	Dirigeable( CMap *map );
	~Dirigeable();

	void Init();					// Initialisation de l'objet g�om�trique
	void Affiche();					// Fonction d'affichage de l'objet g�om�trique
	void Refresh( CGame *game );	// Rafraichissement des donn�es, position, vitesse, ... de l'objet
};

}	// JktMoteur

#endif

