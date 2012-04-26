
#ifndef __JKT__GEO_H
#define __JKT__GEO_H

#include "tinyxml.h"

#include "util/V3D.h"
#include "util/GenRef.h"

/************************************************************************/
/*																		*/
/*			Cette classe est une interface,								*/
/*			elle est la base de tout affichable en 3D.					*/
/*																		*/
/************************************************************************/

class CPlayer;

using namespace JktUtils;

namespace JktMoteur
{

class CMap;
class CIfstreamMap;

class CGeo
{
	static JktUtils::CGenRef GENREF;
	CMap *pMap;			// Navigabilit� vers l'objet parent
	string m_Nom;				// Nom donn� � au g�o
protected:
	unsigned int m_Reference;	// R�f�rence identifiant l'objet (pour la s�rialisation)
public:
		// Constructeurs / destructeur
	CGeo( CMap *map );
	virtual ~CGeo();

		// Fonctions membres
	CMap *getMap();			// Navigabilit� vers l'objet qui contient cet objet (la Map �tant l'objet racine)
	void setName(const string &nom);	// Impl�mente le nom du g�o
	const char *getName();				// Renvoie le nom de l'objet
	unsigned int getReference();	// Renvoie la r�f�rence de l'objet

		// Interface
	virtual void initGL()=0;							// Initialisation de l'objet g�om�trique
	virtual void freeGL()=0;							// Lib�ration des ressources de l'objet dans le contexte OpenGL
	virtual void Init()=0;								// Initialisation de l'objet g�om�trique
	virtual void Affiche()=0;							// Affiche l'objet g�om�trique
	virtual void AfficheSelection(float r,float v,float b)=0;	// Affiche l'objet g�om�trique en couleur unique
	virtual bool Lit(TiXmlElement* el)=0;
//	virtual bool LitFichier( CIfstreamMap &fichier );	// Lit un objet g�o dans un fichier Map
//	virtual bool SaveNameType( ofstream &fichier )=0;	// Sauvegarde du nom du type d'objet g�om�trique
//	virtual bool SaveFichierMap( ofstream &fichier );	// Sauve l'objet g�o dans un fichier Map
	virtual bool Save(TiXmlElement* element) = 0;		// Sauve l'objet g�o dans un fichier Map
	virtual void EchangeXY()=0;							// Echange les coordonn�es X et Y de l'objet
	virtual void EchangeXZ()=0;							// Echange les coordonn�es X et Z de l'objet
	virtual void EchangeYZ()=0;							// Echange les coordonn�es Y et Z de l'objet
	virtual void Scale( float scaleX, float scaleY, float scaleZ )=0;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	virtual void GereContactPlayer( const float pos[3], CPlayer *player )=0;	// G�re les contact entre 'player' et l'objet g�o
	virtual float GereLaser( float pos[3], CV3D &Dir, float dist)=0;	// Voir la d�finition de la fonction

	virtual bool Contact( const float pos[3], float dist );
	virtual const char* toString()=0;	// Description r�sum�e de l'objet
};

}	// JktMoteur

#endif

