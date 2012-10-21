
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "util/GenRef.h"
#include "spatial/IfstreamMap.h"

#include "spatial/geo/Geo.h"

namespace JktMoteur
{
JktUtils::CGenRef CGeo::GENREF;

CGeo::CGeo( CMap *map )
{
	pMap = map;
	m_Reference = GENREF.genRef();	// G�n�re une r�f�rence pour l'objet
}

CGeo::~CGeo()
{
	GENREF.delRef(m_Reference);		// Lib�re la r�f�rence de l'objet
}

CMap *CGeo::getMap()
{	return pMap;	}

unsigned int CGeo::getReference()
{
	return m_Reference;
}

void CGeo::initGL() {}

void CGeo::freeGL() {}

void CGeo::setName( const string &nom )
{
	m_Nom.clear();
	m_Nom = nom;
}

const char *CGeo::getName()
{
	return m_Nom.c_str();
}

bool CGeo::Contact( const float pos[3], float dist )
{
	return false;
}

}	// JktMoteur
