
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>

using namespace std;

#include "SDL.h"
#include "tinyxml.h"

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/Trace.h"
#include "util/TraceMethod.h"
#include "util/Erreur.h"
#include "util/Tableau.cpp"
#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMaker.h"
#include "spatial/IfstreamMap.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"
#include "spatial/light/LightMaker.h"
#include "spatial/Mouve.h"
#include "spatial/geo/Geo.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/geo/SimpleGeo.h"
#include "spatial/geo/SimpleMaterialGeo.h"
#include "spatial/geo/MultiMaterialGeo.h"
#include "spatial/geo/TextureMaterialGeo.h"
#include "spatial/geo/GeoObject.h"
#include "spatial/objet/Porte.h"
#include "spatial/objet/Navette.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "util/V3D.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/geo/EntryPointMaker.h"
#include "ressource/RessourcesLoader.h"

#include "spatial/Map.h"

using namespace JktUtils;

namespace JktMoteur
{

CMap::CMap() {
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap() begin%T", this );
	m_bSelection = false;
	m_Selection = 0;
	_isGlActivated = false;
}

CMap::CMap(const string &nomFichier) throw(JktUtils::CErreur) {
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap(nomFichier=%s) begin%T", nomFichier.c_str(), this );
	if( !Lit(nomFichier) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur � la lecture du fichier MAP : " << nomFichier;
	}
	else {
		cout << endl << "Fichier MAP lu : " << nomFichier;
	}

	m_bSelection = false;
	m_Selection = 0;
	_isGlActivated = false;

	Init();
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap() end%T", this );
}

CMap::~CMap() {
TRACE().p( TRACE_MOTEUR3D, "CMap::~CMap() begin%T", this );

	// Destruction des objets g�o
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		delete *iterGeo;

	m_TabGeo.clear();
	m_TabMouve.clear();

	// Destruction des mat�riaux
	vector<CMaterial*>::iterator iterMat;

	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
		delete *iterMat;

	m_TabMaterial.clear();

	// Destruction des lumi�res
	vector<CLight*>::iterator iterLight;

	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		delete *iterLight;

	m_TabLight.clear();

TRACE().p( TRACE_MOTEUR3D, "CMap::~CMap() end%T", this );
}

void CMap::Affiche() {	// Affiche tous les objets g�o de du MAP
	// Si n�cessaire, initialise les �l�ments OpenGL de la MAP
	if(!_isGlActivated) {
		initGL();
	}

	glEnableClientState( GL_VERTEX_ARRAY );

	vector<CGeo*>::iterator iterGeo;

	if(m_bSelection) {
		int i=0;

		for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
			if(i != m_Selection)
				(*iterGeo)->Affiche();			// Affichage de l'objet g�o
			else
				(*iterGeo)->AfficheSelection(1.0f, 0.0f, 0.0f);

			i++;
		}
	}
	else {
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ ) {
			(*iterGeo)->Affiche();			// Affichage de l'objet g�o
		}
	}

	glDisable( GL_VERTEX_ARRAY );
}

void CMap::add(CGeo* geo) {
	m_TabGeo.push_back(geo);	// Ajoute geo � la liste des objets affichables
}

void CMap::add(CMaterial *mat) {
	m_TabMaterial.push_back(mat);	// Ajoute mat � la liste des objets affichables
}

void CMap::add(CLight *light) {
	m_TabLight.push_back(light);	// Ajoute light � la liste des objets affichables
}

void CMap::incrementeSelection() {
	m_Selection++;

	if(m_Selection >= (int)m_TabGeo.size()) {
		m_Selection = 0;
	}
}

void CMap::decrementeSelection() {
	m_Selection--;

	if(m_Selection < 0) {
		m_Selection = (int)m_TabGeo.size()-1;

		if(m_Selection < 0)
			m_Selection = 0;
	}
}

void CMap::ChangeSelectionMode() {
	m_bSelection = !m_bSelection;
	m_Selection = 0;
}

bool CMap::IsSelectionMode() {
	return m_bSelection;
}

const char* CMap::getSelectedName() {
	CGeo* geo = m_TabGeo[m_Selection];
	return geo->toString();
}

void CMap::merge(CMap& map) {
	vector<EntryPoint>::iterator iterEntryPoint;
	vector<CGeo*>::iterator iterGeo;
	vector<CMouve*>::iterator iterMouve;
	vector<CLight*>::iterator iterLight;
	vector<CMaterial*>::iterator iterMaterial;

	for(iterEntryPoint=map._entryPoints.begin() ; iterEntryPoint!=map._entryPoints.end() ; iterEntryPoint++)
		_entryPoints.push_back(*iterEntryPoint);

	for(iterGeo=map.m_TabGeo.begin() ; iterGeo!=map.m_TabGeo.end() ; iterGeo++)
		m_TabGeo.push_back(*iterGeo);

	for(iterMouve=map.m_TabMouve.begin() ; iterMouve!=map.m_TabMouve.end() ; iterMouve++)
		m_TabMouve.push_back(*iterMouve);

	for(iterLight=map.m_TabLight.begin() ; iterLight!=map.m_TabLight.end() ; iterLight++)
		m_TabLight.push_back(*iterLight);

	for(iterMaterial=map.m_TabMaterial.begin() ; iterMaterial!=map.m_TabMaterial.end() ; iterMaterial++)
		m_TabMaterial.push_back(*iterMaterial);
}

void CMap::add(EntryPoint entryPoint) {
	_entryPoints.push_back(entryPoint);
}

vector<EntryPoint>& CMap::getEntryPointsList() {
	return _entryPoints;
}

void CMap::add( CPorte *porte ) {
	// Une porte est avant tout un objet g�o
	m_TabGeo.push_back( porte );		// Ajoute porte � la liste des objets affichables
	m_TabMouve.push_back( porte );		// Ajoute porte � la liste des objets � rafraichir
}

void CMap::add( CNavette *navette ) {		// Une navette est avant tout un objet g�o
	m_TabGeo.push_back( navette );		// Ajoute porte � la liste des objets affichables
	m_TabMouve.push_back( navette );	// Ajoute porte � la liste des objets � rafraichir
}

void CMap::GereContactPlayer( CPlayer *player ) {
	float pos[3];
	player->getPosition(pos);

	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++)
		(*iterGeo)->GereContactPlayer(pos, player);	// G�re les contacts entre l'objet g�o et le joueur
}

float CMap::GereLaserPlayer( float *pos, CV3D &Dir, float dist ) {
	// Renvoie la distance du premier point de contact entre un rayon laser parti du point 'pos'
	// dans la direction 'Dir' si cette distance est inf�rieure � 'dist', renvoie 'dist' sinon
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		dist = (*iterGeo)->GereLaser( pos, Dir, dist );

	return dist;	// Renvoie la distance du premier contact trouv� entre le laser et une face d'objet g�o
}

void CMap::EchangeXY() {
TRACE().p( TRACE_MOTEUR3D, "CMap::EchangeXY()%T", this );

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeXY();

	// Geo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->EchangeXY();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->EchangeXY();
}

void CMap::EchangeXZ() {
TRACE().p( TRACE_MOTEUR3D, "CMap::EchangeXZ()%T", this );

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeXZ();

	// Geo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->EchangeXZ();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->EchangeXZ();
}

void CMap::EchangeYZ() {
TRACE().p( TRACE_MOTEUR3D, "CMap::EchangeYZ()%T", this );

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeYZ();

	// Geo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->EchangeYZ();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->EchangeYZ();
}

void CMap::Scale(float scaleX, float scaleY, float scaleZ) {
TRACE().p( TRACE_MOTEUR3D, "CMap::Scale(scaleX=%f,sclaeY=%f,scaleZ=%f)%T", scaleX, scaleY, scaleZ, this );

	if(scaleX!=1.0 || scaleY!=1.0 || scaleZ!=1.0) {
		// Entry points
		vector<EntryPoint>::iterator iterEntry;
		for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
			(*iterEntry).Scale(scaleX, scaleY, scaleZ);

		// Geo
		vector<CGeo*>::iterator iterGeo;
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
			(*iterGeo)->Scale( scaleX, scaleY, scaleZ );

		// Lights
		vector<CLight*>::iterator iterLight;
		for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
			(*iterLight)->Scale( scaleX, scaleY, scaleZ );
	}
}

void CMap::translate(float x, float y, float z) {
TRACE().p( TRACE_MOTEUR3D, "CMap::translate(x=%f,y=%f,z=%f)%T", x, y, z, this );

	if(x!=0.0 || y!=0.0 || z!=0.0) {
		// Entry points
		vector<EntryPoint>::iterator iterEntry;
		for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
			(*iterEntry).translate(x, y, z);

		// Geo
		vector<CGeo*>::iterator iterGeo;
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
			(*iterGeo)->translate(x, y, z);

		// Lights
		vector<CLight*>::iterator iterLight;
		for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
			(*iterLight)->translate(x, y, z);
	}
}

bool CMap::Lit(const string &nomFichier) {
	return Lit(*this, nomFichier);
}

bool CMap::Lit(CMap& map, const string &nomFichier) {
	string nomFichierComplet = string(nomFichier);
	bool isResource = JktUtils::RessourcesLoader::getFileRessource(nomFichierComplet);

	// Si le fichier Map n'est pas une resource alors c'est une map classique
	if(!isResource) {
		nomFichierComplet = ".\\map\\" + nomFichierComplet;
	}

	string nomFichierXml = nomFichierComplet + ".map.xml";	// Chemin complet fichier Map XML
	string repertoireBinaires = string(nomFichierComplet);	// Chemin des fichier binaires de la Map
	bool result = false;

	TiXmlDocument document(nomFichierXml.c_str());

	if(document.LoadFile()) {
		// Element de map
		TiXmlElement* elMap = document.FirstChildElement(Xml::MAP);
		if(!elMap)
			throw CErreur(0,"Fichier map corrompu");

		// Lecture des imports de sous-Map
		{
			TiXmlElement* elImport = elMap->FirstChildElement(Xml::IMPORTS);

			if(elImport) {
				for(TiXmlElement* el=elImport->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::IMPORT, el->Value())) {
						string erreur = "Fichier MAP corrompu : '";
						erreur += Xml::IMPORT;
						erreur += "' attendu, '";
						erreur += el->Value();
						erreur += "' recu";
						throw CErreur(0, erreur);
					}

					const char* subMapName = el->Attribute(Xml::NOM);	// Lecture nom de la Map � importer

					if(!subMapName)
						throw CErreur(0, "Fichier Map corrompu : Nom de la sous-Map a importer manquant");

					// Lecture de la translation � appliquer � la Map
					float translation[3];
					Xml::Lit3fv(el, Xml::TRANSLATE, Xml::X, Xml::Y, Xml::Z, translation);

					// Lecture du scaling � appliquer � la Map
					float scaling[3];
					Xml::Lit3fv(el, Xml::SCALE, Xml::X, Xml::Y, Xml::Z, scaling);

					// Lecture de la Map
					CMap subMap;
					Lit(subMap, string(subMapName));

					subMap.translate(translation[0], translation[1], translation[2]);
					subMap.Scale(scaling[0], scaling[1], scaling[2]);

					// Fusion des Map
					map.merge(subMap);
					subMap.m_TabGeo.clear();
					subMap.m_TabMouve.clear();
					subMap.m_TabLight.clear();
					subMap.m_TabMaterial.clear();
				}
			}
		}

		// Lecture des point d'entr�e des joueurs
		{
			TiXmlElement* elEntry = elMap->FirstChildElement(Xml::ENTRYPOINTS);

			if(elEntry) {
				for(TiXmlElement* el=elEntry->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::ENTRYPOINT, el->Value())) {
						string erreur = "Fichier MAP corrompu : '";
						erreur += Xml::ENTRYPOINT;
						erreur += "' attendu, '";
						erreur += el->Value();
						erreur += "' recu";
						throw CErreur(0, erreur);
					}

					EntryPoint* entry = EntryPointMaker::Lit(el);

					if(entry)
						map.add(*entry);
				}
			}
		}

		// Lecture des mat�riaux
		TiXmlElement* elMat = elMap->FirstChildElement(Xml::MATERIAUX);

		if(elMat) {
			for(TiXmlElement* el=elMat->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::MATERIAU, el->Value())) {
					string erreur = "Fichier MAP corrompu : '";
					erreur += Xml::MATERIAU;
					erreur += "' attendu, '";
					erreur += el->Value();
					erreur += "' recu";
					throw CErreur(0, erreur);
				}

				CMaterial* mat = CMaterialMaker::Lit(el, repertoireBinaires);

				if(mat)
					map.add(mat);
			}
		}

		// Lecture des lumi�res
		TiXmlElement* elLight = elMap->FirstChildElement(Xml::LUMIERES);

		if(elLight) {
			for(TiXmlElement* el=elLight->FirstChildElement(); el!=0; el=el->NextSiblingElement())
			{
				if(strcmp(Xml::LUMIERE, el->Value()))
				{
					string erreur = "Fichier map corrompu : '";
					erreur += Xml::LUMIERE;
					erreur += "' attendu, '";
					erreur += el->Value();
					erreur += "' recu";
					throw CErreur(0, erreur);
				}

				CLight* lum = CLightMaker::Lit(el);

				if(lum)
					map.add(lum);
			}
		}

		// Lecture des objets g�om�triques
		TiXmlElement* elGeo = elMap->FirstChildElement(Xml::GEOS);

		if(elGeo) {
			for(TiXmlElement* el=elGeo->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::GEO, el->Value())) {
					string erreur = "Fichier map corrompu : '";
					erreur += Xml::GEO;
					erreur += "' attendu, '";
					erreur += el->Value();
					erreur += "' recu";
					throw CErreur(0, erreur);
				}

				CGeo* geo = CGeoMaker::Lit(el, &map);

				if(geo)
					map.add(geo);
			}
		}

		result = true;
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Ouverture impossible : " << nomFichierXml;
	}

	return result;
}

bool CMap::Init() throw(JktUtils::CErreur) {	// Initialisation de la CMap
	// Initialisation des object g�om�triques
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->Init();
	}

	return true;
}

bool CMap::initGL() {
	// Letcure des fichiers de texture
	vector<CMaterial*>::iterator iterMat;

	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ ) {
		CMaterial* material = *iterMat;
		material->initGL();
	}

	// Initialisation des object g�om�triques dans le contexte OpenGL
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->initGL();
	}

	_isGlActivated = true;	// Indique que les �l�ments OpenGL de la MAP ont bien �t� initialis�s

	return true;
}

bool CMap::freeGL() {
	// Initialisation des object g�om�triques dans le contexte OpenGL
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->freeGL();
	}

	return true;
}

/*bool CMap::SaveFichierMap( const string &nomFichier )
{
TRACE().p( TRACE_MOTEUR3D, "CMap::SaveFichierMap() %T", this );

		// CREATION DES FICHIERS
	string nomFichierMap = "./map/" + nomFichier + ".map";

	ofstream fichier( nomFichierMap.c_str() );

	fichier << "Start";		// Indique le d�but d'une description de Map ou de sous-Map

		// SAUVEGARDE DES MATERIAUX
	vector<CMaterial*>::iterator iterMat;
	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
		(*iterMat)->SaveFichierMap(fichier);		// Sauvegarde du mat�riau

		// SAUVEGARDE DES LUMIERES
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->SaveFichierMap( fichier );

		// SAUVEGARDE DES OBJETS GEOMETRIQUES
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
	{
		(*iterGeo)->SaveNameType( fichier );		// Sauvegarde du nom du type d'objet g�o
		(*iterGeo)->SaveFichierMap( fichier );		// Sauvegarde des param�tres de l'objet
	}

	fichier.close();
	cout << endl << "Sauvegarde du fichier MAP Ok !!";

TRACE().p( TRACE_MOTEUR3D, "CMap::SaveFichierMap() Sauvegarde du fichier MAP Ok%T", this );
	return true;
}*/

bool CMap::Save(const string nomFichier) {
TRACE().p( TRACE_MOTEUR3D, "CMap::Save() %T", this );

		// CREATION DES FICHIERS
	string nomFichierMap = "./map/" + nomFichier + ".map.xml";

	// Initialisation du doc XML
	TiXmlDocument document;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","");
	document.LinkEndChild(decl);

	TiXmlElement *elMap = new TiXmlElement("Map");
	document.LinkEndChild(elMap);

	// Sauvegarde des points d'entr�e des joueurs
	{
		TiXmlElement *elEntryPoint = new TiXmlElement(Xml::ENTRYPOINTS);
		elMap->LinkEndChild(elEntryPoint);
		vector<EntryPoint>::iterator iterEntryPoint;

		for( iterEntryPoint=_entryPoints.begin() ; iterEntryPoint!=_entryPoints.end() ; iterEntryPoint++ )
			(*iterEntryPoint).Save( elEntryPoint );		// Sauvegarde des param�tres de l'objet
	}

	// Sauvegarde des materiaux
	{
		TiXmlElement *elMat = new TiXmlElement(Xml::MATERIAUX);
		elMap->LinkEndChild(elMat);
		vector<CMaterial*>::iterator iterMat;

		for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
			(*iterMat)->Save(elMat);		// Sauvegarde du mat�riau
	}

	// Sauvegarde des lumieres
	{
		TiXmlElement *elLum = new TiXmlElement(Xml::LUMIERES);
		elMap->LinkEndChild(elLum);
		vector<CLight*>::iterator iterLight;

		for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
			(*iterLight)->Save( elLum );
	}

	// Sauvegarde des objets geometriques
	{
		TiXmlElement *elGeo = new TiXmlElement(Xml::GEOS);
		elMap->LinkEndChild(elGeo);
		vector<CGeo*>::iterator iterGeo;

		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
			(*iterGeo)->Save( elGeo );		// Sauvegarde des param�tres de l'objet
	}

	document.SaveFile(nomFichierMap.c_str());

	cout << endl << "Sauvegarde du fichier MAP Ok !!";

TRACE().p( TRACE_MOTEUR3D, "CMap::SaveFichierMap() Sauvegarde du fichier MAP Ok%T", this );
	return true;
}

bool CMap::Contact(const float pos[3], const float dist) {
	bool var = false;	// Pas de contact par d�faut
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		var = (*iterGeo)->Contact( pos, dist );

		if(var)	// Si un triangle a �t� trouv� � une distance inf�rieure � 'dist' de la position 'pos'
			break;
	}

	return var;
}

void CMap::Refresh(CGame *game) {
	vector<CMouve*>::iterator iterMouve;

	for(iterMouve=m_TabMouve.begin() ; iterMouve!=m_TabMouve.end() ; iterMouve++)
		(*iterMouve)->Refresh(game);
}

void CMap::afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index) {
	if(index < firstIndex + (nbrX * nbrY)) {
		if(material->Type() == CMaterial::MAT_TYPE_TEXTURE) {
			if(index > firstIndex) {
				CMaterialTexture *matTexture = (CMaterialTexture*)material;
				matTexture->Active();

				int x1 = x + posX*tailleX/nbrX;
				int x2 = x1 + tailleX/nbrX;
				int y1 = y + posY*tailleY/nbrY;
				int y2 = y1 + tailleY/nbrY;


				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2i(x1, 	y1);

					glTexCoord2f(1.0f, 0.0f);
					glVertex2i(x2, 	y1);

					glTexCoord2f(1.0f, 1.0f);
					glVertex2i(x2, 	y2);

					glTexCoord2f(0.0f, 1.0f);
					glVertex2i(x1, 	y2);
				glEnd();

				glDisable(GL_TEXTURE_2D);
				glColor3f(1.0f, 1.0f, 1.0f);
				glLineWidth(2);

				glBegin(GL_LINE_LOOP);
					glVertex2i(x1, 	y1);
					glVertex2i(x2, 	y1);
					glVertex2i(x2, 	y2);
					glVertex2i(x1, 	y2);
				glEnd();

				if(++posX >= nbrX) {
					posX = 0;
					posY++;
				}
			}

			index++;
		}
		else if( material->Type() == CMaterial::MAT_TYPE_SIMPLE ) {
		}
		else if( material->Type() == CMaterial::MAT_TYPE_MULTI ) {
			// Initialisation pour la texture
			CMaterialMulti *matMulti = (CMaterialMulti*)material;

			for(int i=0 ; i<matMulti->NbrTex() ; i++) {
				CMaterial* sousMat = matMulti->getMat(i);
				afficheMaterial(sousMat, x, y, tailleX, tailleY, nbrX, nbrY, firstIndex, posX, posY, index);
			}
		}
		else {
			cerr << endl << __FILE__ << ":" << __LINE__ << " CMap::afficheToutesTextures : Materiau de type inconnu";
		}
	}
}

vector<CLight*>& CMap::getLights() {
	return m_TabLight;
}

void CMap::afficheToutesTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex) {
	int posX = 0;
	int posY = 0;
	int index = firstIndex;
	vector<CMaterial*>::iterator iter;

	for(iter=m_TabMaterial.begin() ; iter!=m_TabMaterial.end() ; iter++) {
		CMaterial* material = *iter;
		afficheMaterial(material, x, y, tailleX, tailleY, nbrX, nbrY, firstIndex, posX, posY, index);
	}
}

}	// JktMoteur
