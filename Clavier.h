
#ifndef __CLAVIER_H
#define __CLAVIER_H

class CClavier
{
public :
	bool m_bIndic;		// Indique si une requ�te de d�placement clavier existe pour le prochain affichage
	float m_fAvance;	// Indique si le joueur veut avancer
	float m_fDroite;	// Indique si le joueur veut aller vers la droite
	float m_fMonte;		// Indique si le joueur veut monter

		// CONSTRUCTEURS
	CClavier();
	
	void reset();
};

#endif


