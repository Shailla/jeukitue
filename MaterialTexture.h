#ifndef __JKT__MATERIALTEXTURE_H
#define __JKT__MATERIALTEXTURE_H

#include <iostream>
#include <string>

#include "Material.h"
#include "util/Erreur.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CMaterialTexture:public CMaterial
{
public:
	unsigned int texName;	// ID de la texture fourni par openGL

	std::string m_FichierTexture;		// Nom du fichier de la texture associ�e

		// Constructeurs / Destructeurs
	CMaterialTexture();
	CMaterialTexture( const CMaterial &mat );
	~CMaterialTexture();

		// Fonctions membres
	void LitTexture() throw(JKT_PACKAGE_UTILS::CErreur); // Lit le fichier de texture associ� au mat�riau
	bool Lit(TiXmlElement* el, string &repertoire);
	bool LitFichier(CIfstreamMap &fichier);
	bool SaveFichierMap(std::ofstream &fichier);
	bool Save(TiXmlElement* element);
	void Active();
	void Desactive();
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

