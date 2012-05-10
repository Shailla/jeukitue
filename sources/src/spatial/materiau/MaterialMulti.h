#ifndef __JKT__MATERIALTREE_H
#define __JKT__MATERIALTREE_H

#include <iostream>

#include "spatial/materiau/MaterialTexture.h"

#include "spatial/materiau/Material.h"

namespace JktMoteur
{

class CMaterialMulti:public CMaterial
{
	int m_NbrTex;				// Nombre de sous-mat�riaux

public:
	CMaterial** m_TabMat;		// Tableau des sous-mat�riaux

		// Constructeurs / Destructeurs
	CMaterialMulti();
	~CMaterialMulti();

		// Fonctions membres
	void initGL();
	void NbrTex(int nbr);		// Impl�mente le nombre de sous-mat�riaux
	int NbrTex();				// Renvoie le nombre de sous-mat�riaux
	CMaterial* getMat(int i);	// Renvoie le sous-mat�riau d'index i
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);
	bool Lit(TiXmlElement* element, string &repertoire);
	const char* toString();
};

}	// JktMoteur

#endif
