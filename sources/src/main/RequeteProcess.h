
#ifndef __JKT__REQUETEPROCESS_H
#define __JKT__REQUETEPROCESS_H

#include <string>
#include "SDL.h"

using namespace std;


class CRequeteProcess
{
public:
	enum OUVRE_MAP_LOCALE_ETAPES {
		OMLE_AUCUNE,					// Aucune ouverture de MAP locale n'est en cours
		OMLE_DEMANDE,					// L'ouverture d'une MAP locale a �t� demand�e ==> Fermeture MAP en cours et lancement ouverture de la nouvelle MAP
		OMLE_OUVERTURE_EN_COURS,		// Chargement en cours des donn�es de la nouvelle MAP
		OMLE_OUVERTURE					// Finalisation ouverture de la nouvelle MAP
	};

private:
	bool _bOpenMap;						// Indique une demande d'ouvrir la MAP nomm�e dans _MapAOuvrir
	OUVRE_MAP_LOCALE_ETAPES _openMapLocalEtape;	// Indique une demande d'ouvrir la MAP nomm�e dans _MapAOuvrir
	string _MapAOuvrir;					// Indique le nom du fichier Map � ouvrir
	SDL_mutex *_mutex;
	bool _bTakePicture;					// Indique une demande de prise de photo de la sc�ne
public:
	CRequeteProcess();
	~CRequeteProcess();

	void setOuvreMapLocal( string nomMAP );					// Inscrit une demande d'ouverture de MAP
	void setOuvreMap( string nomMAP );						// Inscrit une demande d'ouverture de MAP
	bool isOuvreMap();										// Int�roge sur la pr�sence d'une demande d'ouverture de MAP
	void setOuvreMapLocaleEtape(OUVRE_MAP_LOCALE_ETAPES etape);
	int getOuvreMapLocaleEtape();		// Int�roge sur la pr�sence d'une demande d'ouverture de MAP en jeu local
	string getOuvreMap();									// Renvoie le nom de la MAP � ouvrir et d�truit la demande

	void setTakePicture();									// Inscrit une demande de prise de photo de la sc�ne
	bool isTakePicture();									// Int�roge sur la pr�sence d'un demande de prise de photo
};

#endif

