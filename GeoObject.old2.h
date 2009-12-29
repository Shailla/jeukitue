
#ifndef __JKT__GEOOBJECT_H
#define __JKT__GEOOBJECT_H

/************************************************************/
/*															*/
/*			Objet affichable 3D classique					*/
/*			et le plus g�n�ral qui soit.					*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

using namespace std;

#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/materiau/MaterialTexture.h"

#include "spatial/geo/Geo.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CC3D;
class CMap;

class CChanTex
{
public:
	float *TexVertex;
	int *TexFaces;

	CChanTex() {
		TexVertex = 0;
		TexFaces = 0;
	}

	~CChanTex() {
		if( TexVertex )
			delete[] TexVertex;
		if( TexFaces )
			delete[] TexFaces;
	}
};

class CGeoObject:public CGeo
{
private:
	int m_OffsetMateriaux;		// Sert lors de la lecture des r�f�rences mat�riaux
	CMaterial::MAT_TYPE m_TypeMateriau;	// Type du mat�riau associ� (standard, multi,...)
protected:
		// Param�tres d'initialisation
	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonn�es du pav� englobant l'objet g�o
	float m_Color[3];		// Couleur de l'objet g�o
	bool m_bSolid;				// Indique si l'objet est solide ou non
	float m_Centre[3];			// Centre de la sph�re englobant l'objet
	float m_Rayon;				// Rayon de la sph�re englobant l'objet

	float testContactTriangle( unsigned int i, const float *pos, float dist );

public:
	bool m_bMaterialTexture;		// Doit �tre � "true" si l'objet g�o est associ� � un mat�riau
	int m_MaterialTexture;			// R�f�rence du mat�riau associ�
	CMaterial *m_Material;		// Mat�riau utilis�
		// Nombres d'�l�ments des tableaux
	int m_NumVertex;			// Nbre de sommets
	int m_NumFaces;				// Nbre d'index de sommets
	int m_NumTexVertex;			// Nbre de sommets de texture, "0" si pas de texture associ�e
	int m_NumTexFaces;			// Nbre d'index de sommets de texture

	float *m_TabVertex;			// Pointeur sur le tableau de sommets
	int *m_TabFaces;			// Pointeur sur le tableau d'index des sommets
	int *m_TabSubMat;			// Index des �ventuels sous-mat�riau
	float *m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs pr�liminaires � la gestion des contacts)
	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux
	map<int,CChanTex*> TabTex;	// Contient tous les canaux de texture

		//Constructeurs / Destructeur
	CGeoObject( CMap *Map, unsigned int nbrVertex, unsigned int nbrFaces );
	CGeoObject( CMap *Map );

	~CGeoObject();

		//Fonctions membres
private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet g�om�trique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet g�o

	void AfficheWithMaterialMultiTexture( CMaterialMulti *mat );
	void AfficheWithMaterialTexture( CMaterialTexture *mat );
	void AfficheWithMaterialSimple( CMaterial *mat );

public:
	void EchangeXY();			// Echange les coordonn�es X et Y de l'objet
	void EchangeXZ();			// Echange les coordonn�es X et Z de l'objet
	void EchangeYZ();			// Echange les coordonn�es Y et Z de l'objet
	void Scale( float scaleX, float scaleY, float scaleZ );	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void setMaterial( int matRef );					// Associe l'objet au mat�riau de r�f�rence matRef
	void Color( float r, float g, float b );		// d�fini la couleur de l'objet
	bool TestContactPave( const float pos[3], float dist );	// 'pos' est-il dans le pav� constitu� des distances min/max de l'objet g�o
	void GereContactPlayer( const float pos[3], CPlayer *player );
	float GereLaser( float pos[3], CV3D &Dir, float dist);	// Voir la d�finition de la fonction

	void setVertex( float *tab, int num );		// Impl�mente les sommets
	void setFaces( int *tab, int num );			// Impl�mente les indices de sommets
	void setNormalVertex( float *tab );			// Impl�mente les normales aux sommets
	void setSubMat( int *tab );					// Impl�mente les r�f de sous mat�riau

		// Fonctions pour l'interface CGeo
	bool LitFichier( CIfstreamMap &fichier );			// Lit un objet g�o dans un fichier Map
	bool LitFichierGeoObject( CIfstreamMap &fichier );	// Lit un objet g�o dans un fichier
	bool SaveNameType( ofstream &fichier );				// Sauve le nom du type d'objet g�om�trique
	bool SaveFichierMap( ofstream &fichier );			// Sauve l'objet g�o dans un fichier Map
	void OffsetMateriau( int offset );					// D�cale la r�f�rence mat�riau de l'offset

	bool Contact( const float pos[3], float dist );

	void Affiche();								// Affiche cet objet g�o
	void Init();								// Initialisation de l'objet
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

