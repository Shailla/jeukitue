
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <vector>

using namespace std;

#include "util/Tableau.h"
#include "util/Erreur.h"
#include "util/V3D.h"

class CGame;
class CPlayer;

using namespace JktUtils;

namespace JktMoteur {
class CGeo;
class CLight;
class CMouve;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;
class EntryPoint;

class CMap {
	string _name;							// Nom de la Map ("Monde" par exemple pour "Monde.map.xml")
	string _filename;						// Nom du fichier de la Map (par exemple "Monde.map.xml")
	string _binariesDirectory;				// R�pertoires des binaires de la Map (textures, plugins, ...)
	vector<CGeo*> m_TabGeo;					// Liste des objets g�om�triques
	vector<CMouve*> m_TabMouve;				// Liste des objets n�cessitant une actualisation (portes,...)
	vector<EntryPoint> _entryPoints;		// Liste des points d'entr�e des joueurs sur la Map
	vector<CLight*> m_TabLight;				// Liste des lumi�res
	vector<string> _plugins;				// Liste des plugins de la Map

	bool Init() throw(JktUtils::CErreur);	// Initialisation de la CMap
	bool Lit(const string &nomFichier);
	static bool Lit(CMap& map, const string &mapName);

	int m_Selection;						// Object g�o s�lectionn�
	bool m_bSelection;						// Indique si le mode s�lection est actif ou non

	bool _isGlActivated;					// Indique si les �l�ments OpenGL de la MAP ont �t� initialis�s
	bool _isPluginsActivated;				// Indique si les plugins de la MAP ont �t� initialis�s

	void afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index);
public:
	vector<CMaterial*> m_TabMaterial;		// Liste des mat�riaux A VOIR : devrait �tre membre priv�

		// Constructeurs / destructeur
	CMap();
	CMap(const string &nomFichier) throw(JktUtils::CErreur);
	~CMap();

		// Fonctions membres
	void initPlugins();	// Chargement ou ex�cution des plugins de la Map
	void freePlugins();
	bool initGL();		// Initialisation dans le contexte OpenGL
	bool freeGL();

//	bool SaveFichierMap(const string &nomFichier);		// Sauvegarde du CMap dans un fichier *.map
	bool Save(const string nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

	void Affiche();						// Affiche l'ensemble des �l�ments 3D de cette Map
	void Refresh( CGame *game );		// Rafraichissement des classes list�es dans m_TabMouve

	void add(CGeo *geo);			// Ajoute un GeoObject � la map
	void add(CMaterial *mat);		// Ajoute un mat�riau � la map
	void add(CLight *light);		// Ajoute une lumi�re � la map

	void add(CPorte *porte);		// Ajoute une porte � la map
	void add(CNavette *navette);	// Ajoute une navette � la map

	bool Contact(const float pos[3], const float dist);	// Indique s'il y a un triangle � une distance inf�rieure � 'dist' de la position 'pos'

	void EchangeXY();							// Echange les coordonn�es X et Y des objets g�o du map
	void EchangeXZ();							// Echange les coordonn�es X et Z des objets g�o du map
	void EchangeYZ();							// Echange les coordonn�es Y et Z des objets g�o du map
	void Scale(float scaleX, float sclaeY, float scaleZ);	// Homoth�tie la Map (coordonn�es multipli�es par scale)
	void translate(float x, float y, float z);				// Translation de la Map selon x, y, z
	/** Int�gre tous les �l�ments d'une autre Map dans celle-ci. */
	void merge(CMap& map);
		// A VOIR Fonctions qui n'ont rien � faire ici
	void GereContactPlayer(CPlayer *player);								// G�re tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float *pos, JktUtils::CV3D &Dir, float dist );	// Envoie d'un laser sur la map

	vector<CLight*>& getLights();

	/**
	 * Affiche toutes les textures de la MAP dans un rectangle dont le coin bas gauche est en (x,y) et les dimensions sont (tailleX, tailleY).
	 *
	 * @param x position horizontale du coin bas gauche du rectangle dans lequel les texture vont �tre affich�es
	 * @param y position verticale du coin bas gauche du rectangle dans lequel les texture vont �tre affich�es
	 * @param tailleX taille horizontale du rectangle dans lequel les texture vont �tre affich�es
	 * @param tailleY taille verticale du rectangle dans lequel les texture vont �tre affich�es
	 * @param nbrX nombre de textures sur une ligne
	 * @param nbrY nombre de textures sur une colonne
	 * @param firstIndex num�ro de la premi�re texture � afficher
	 */
	void afficheToutesTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex);

		// Mode s�lection
	void incrementeSelection();	// S�lectionne l'objet g�o suivant
	void decrementeSelection();	// S�lectionne l'objet g�o pr�c�dent
	void ChangeSelectionMode();
	const char* getSelectedName();
	bool IsSelectionMode();

	// Points d'entr�e des personnages joueurs
	void add(EntryPoint entryPoint);
	vector<EntryPoint>& getEntryPointsList();
};

}	// JktMoteur

#endif

