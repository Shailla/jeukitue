
#ifndef __JKT__REQUETEPROCESS_H
#define __JKT__REQUETEPROCESS_H

#include <string>
using namespace std;

class CRequeteProcess
{
	bool m_bOpenMap;		// Indique une demande d'ouvrir la MAP nomm�e dans m_MapAOuvrir
	string m_MapAOuvrir;	// Indique le nom du fichier Map � ouvrir

	bool m_bTakePicture;	// Indique une demande de prise de photo de la sc�ne
public:
	CRequeteProcess();
	~CRequeteProcess();

	void setOuvreMap( string nomMAP );	// Inscrit une demande d'ouverture de MAP
	bool isOuvreMap();					// Int�roge sur la pr�sence d'une demande d'ouverture de MAP
	string getOuvreMap();				// Renvoie le nom de la MAP � ouvrir et d�truit la demande

	void setTakePicture();				// Inscrit une demande de prise de photo de la sc�ne
	bool isTakePicture();				// Int�roge sur la pr�sence d'un demande de prise de photo
};

#endif

