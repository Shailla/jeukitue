
#ifndef __JKT__MULTIMATERIALGEO_H
#define __JKT__MULTIMATERIALGEO_H

/************************************************************/
/*															*/
/*			Objet affichable 3D simple associ� � un			*/
/*			mat�riau unique. Faces non-index�es,			*/
/*			peut �tre �clair�.								*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

using namespace std;

#include "ChanTex.h"
#include "Geo.h"
#include "GeoMaker.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CMultiMaterialGeo:public CGeo
{
	int m_OffsetMateriaux;			// Sert lors de la lecture des r�f�rences mat�riaux
	string tostring;
	CMaterialMulti* m_Material;		// Mat�riau multiple associ�
public:
	static const char* identifier;
	CMultiMaterialGeo(CMap* map, const string& name, CMaterialMulti* mat, unsigned int nbrfaces, float* vertex, float* normals, float* texvertex, std::map<int,int> &canauxnumbers, bool solid=true);
	CMultiMaterialGeo(CMap *map);
	void Init();					// Initialisation de l'objet
private:
	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonn�es du pav� englobant l'objet g�o
	float m_Centre[3];			// Centre de la sph�re englobant l'objet
	float m_Rayon;				// Rayon de la sph�re englobant l'objet
	float* m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs pr�liminaires � la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;				// Nbre de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets
	int getOffsetMateriau() throw(CErreur);
		// Canaux et coordon�es de texture
	map<int,int> m_CanauxNumbers;	// first=num�ro de canal ; second=nombre de faces du canal
	float* m_TabTexVertex;			// Liste des coordonn�es de texture des canaux
		// VBO
	enum {	VBO_VERTEX=0, VBO_NORMAUX, VBO_TEXVERTEX };
	static const int VBO_BUFFER_SIZE = 3;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];
	void initVBO();
	void freeVBO();
public:
		// Destructeur
	~CMultiMaterialGeo();

	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux
private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet g�om�trique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet g�o
	void AfficheNormals();
public:
	void EchangeXY();			// Echange les coordonn�es X et Y de l'objet
	void EchangeXZ();			// Echange les coordonn�es X et Z de l'objet
	void EchangeYZ();			// Echange les coordonn�es Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ);	// Homoth�tie pond�r�e selon X, Y et Z de l'objet

	bool TestContactPave(const float pos[3], float dist);	// 'pos' est-il dans le pav� constitu� des distances min/max de l'objet g�o
	void GereContactPlayer(const float pos[3], CPlayer *player);
	float GereLaser(float pos[3], CV3D &Dir, float dist);	// Voir la d�finition de la fonction

	void setMaterial(int matRef);	// Associe l'objet au mat�riau de r�f�rence matRef
	void setNormalVertex(float *tab);			// Impl�mente les normales aux sommets
	void setSubMat(int *tab);					// Impl�mente les r�f de sous mat�riau
	void setVertex(int num, float *tab);		// Impl�mente les sommets
	void setTexVertex(float *tab);				// Impl�mente les sommets de texture
	void setOffsetMateriau(int offset);			// D�cale la r�f�rence mat�riau de l'offset

		// Fonctions pour l'interface CGeo
	//bool LitFichier(CIfstreamMap &fichier);			// Lit un objet g�o dans un fichier Map
	//bool SaveNameType(ofstream &fichier);				// Sauve le nom du type d'objet g�om�trique
	//bool SaveFichierMap(ofstream &fichier);			// Sauve l'objet g�o dans un fichier Map
	bool Save(TiXmlElement* element);
	bool Lit(TiXmlElement* el);

	bool Contact( const float pos[3], float dist );

	void Affiche();									// Affiche cet objet g�o
	void AfficheSelection(float r,float v,float b);	// Affiche l'objet en couleur unique
	const char* toString();
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

