#ifndef __JKT__MOTEURPARTICULESNEIGE_H
#define __JKT__MOTEURPARTICULESNEIGE_H

#include "util/V3D.h"
#include "spatial/MoteurParticules.h"

namespace JktMoteur
{

class CMoteurParticulesNeige : public CMoteurParticules {
		// Variables
	unsigned int nbrParticules;		// Nombre max de particules
	float masse;					// Masse du centre attracteur
	CParticule *ListeParticules;	// Liste des particules
	unsigned int texName;			// Texture de la particule
	unsigned lastTempsNew;			// Date de la derni�re cr�ation d'une particule

	/** Calcule une position al�atoire pour une nouvelle particule. */
	CV3D getRandomParticulePosition();
public:
	/** Position du centre du moteur de particules. */
	JktUtils::CV3D _centre;
	/** Taille du moteur de particules. */
	JktUtils::CV3D _taille;
	JktUtils::CV3D _vitesse;
		// Fonctions membres
private:
	void GenereTextureParticule();

public:
	/**
	 * Constructeur.
	 *
	 * @param nbr nombre de particules dans le moteur
	 * @param position position du centre du moteur de particules dans le sc�ne OpenGL
	 * @param taille vecteur dimension du parall�l�pip�de dans lequel sont affich�es les particules
	 */
	CMoteurParticulesNeige(unsigned int nbr, CV3D position, CV3D taille);

	/** Affiche la neige. */
	void Affiche();		// Affiche l'ensemble des particules
};

}	// JktMoteur

#endif
