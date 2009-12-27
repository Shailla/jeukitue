
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <vector>

#include "V3D.h"
#include "Tableau.h"
#include "Erreur.h"

class CGame;
class CPlayer;

using namespace JKT_PACKAGE_UTILS;

namespace JKT_PACKAGE_MOTEUR3D
{
class CGeo;
class CLight;
class CMouve;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;

class CMap
{
	vector<CGeo*> m_TabGeo;				// Liste des objets g�om�triques
	vector<CMouve*> m_TabMouve;			// Liste des objets n�cessitant une actualisation (portes,...)

	bool Init() throw(JKT_PACKAGE_UTILS::CErreur);								// Initialisation de la CMap
//	bool LitFichier(const string &nomFichier);	// Lecture de la CMap depuis un fichier *.map
	bool Lit(const string &nomFichier);

	int m_Selection;	// Object g�o s�lectionn�
	bool m_bSelection;	// Indique si le mode s�lection est actif ou non
public:
	vector<CLight*> m_TabLight;			// Liste des lumi�res
	vector<CMaterial*> m_TabMaterial;	// Liste des mat�riaux A VOIR : devrait �tre membre priv�
	vector<CV3D> m_EntreeJoueurs;		// Liste des points d'entr�e des joueurs sur la Map

		// Constructeurs / destructeur
	CMap();
	CMap(const string &nomFichier ) throw(JKT_PACKAGE_UTILS::CErreur);
	~CMap();

		// Fonctions membres
//	bool SaveFichierMap(const string &nomFichier);		// Sauvegarde du CMap dans un fichier *.map
	bool Save(const string &nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

	void Affiche();						// Affiche l'ensemble des �l�ments 3D de cette Map
	void Refresh( CGame *game );		// Rafraichissement des classes list�es dans m_TabMouve

	void Add(CGeo *geo);			// Ajoute un GeoObject � la map
	void Add(CMaterial *mat);		// Ajoute un mat�riau � la map
	void Add(CLight *light);		// Ajoute une lumi�re � la map

	void Add(CPorte *porte);		// Ajoute une porte � la map
	void Add(CNavette *navette);	// Ajoute une navette � la map

	bool Contact(const float pos[3], float dist);	// Indique s'il y a un triangle � une distance inf�rieure � 'dist' de la position 'pos'

	void EchangeXY();							// Echange les coordonn�es X et Y des objets g�o du map
	void EchangeXZ();							// Echange les coordonn�es X et Z des objets g�o du map
	void EchangeYZ();							// Echange les coordonn�es Y et Z des objets g�o du map
	void Scale(float scaleX, float sclaeY, float scaleZ);	// Homoth�tie sur les objets
														// g�o (ils sont multipli�s par scale)
		// A VOIR Fonctions qui n'ont rien � faire ici
	void GereContactPlayer(CPlayer *player);	// G�re tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float *pos, CV3D &Dir, float dist );	// Envoie d'un laser sur la map

	void afficheToutesTextures(int x, int y);

		// Mode s�lection
	void incrementeSelection();	// S�lectionne l'objet g�o suivant
	void decrementeSelection();	// S�lectionne l'objet g�o pr�c�dent
	void ChangeSelectionMode();
	const char* getSelectedName();
	bool IsSelectionMode();
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

