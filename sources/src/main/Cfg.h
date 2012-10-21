

#ifndef __JKT__CFG_H
#define __JKT__CFG_H

#define VERSION_DAY		31
#define VERSION_MONTH	10
#define VERSION_YEAR	2004

#include <string>
#ifdef WIN32
	#include <windows.h>
#endif

#include "SDL.h"
#include "GL/gl.h"
#include "glext.h"

using namespace std;

class CCfg {

	class CComID {
	public:
		SDLKey key;		// Touche du clavier correspondant
		int mouse;		// Touche de la souris correspondant
	};

	class CAudio {
	public :
		int m_Output;
		int m_Mixer;
		int m_Driver;
		int m_DriverRecord;

		bool Init();
	};

	class CCommandes {
	public:
		CComID Avancer;		// Avancer
		CComID Reculer;		// Reculer
		CComID Gauche;		// Aller � gauche
		CComID Droite;		// Aller � droite
		CComID Tir1;		// Tir primaire
		CComID Tir2;		// Tir secondaire
		CComID Monter;		// Monter
		const char* resolve(CComID com);			// Retourne la touche ou le bouton en texte
		const char* resolve(Uint8 mouse);			// Retourne le bouton souris en texte
		const char* resolve(const SDLKey sym);		// Retourne la touche en texte
	};

	class CDisplay {
		friend class CCfg;
		bool m_bFullscreen;
		void InitSDL();
		void InitOpenGL();
        void InitAgar();
		bool chargeGLExtension(const char* ext, string& extensions);
	public :
		int X;
		int Y;
		bool Fullscreen() const		{	return m_bFullscreen;	}
		void Fullscreen(bool on)	{	m_bFullscreen = on;		}
		void Init();
		void ChangeVideoSize(int x, int y);
	};

	class CCentralisateur {
		friend class CCfg;
	protected:
		Uint16 m_Port;				// Port � utiliser
		string m_IpServer;			// Adresse IP du serveur (s'il s'agit d'un client)
	public:
		Uint16 getPort() const			{ return m_Port; }
		void setPort( Uint16 port )		{ m_Port = port; }
		string getIp() const		{ return m_IpServer; }
		void setIp( string ip )		{ m_IpServer = ip; }
	};

	class CReseau {		// Informations sur la config r�seau
		friend class CCfg;
	protected:
		Uint16 m_Port;			// Port � utiliser
		string m_IpServer;		// Adresse IP du serveur (s'il s'agit d'un client)
	public:
		bool serveur;	// true s'il s'agit d'un serveur, false si c'est un client
		Uint16 getPort() const			{ return m_Port;	}
		string getIpServer() const		{ return m_IpServer;		}
		void setPort( Uint16 port )		{ m_Port = port;	}
		void Init();
	};

	class CJoueur {		// Informations du joueur
	public:
		string nom;					// Nom du joueur
		string mapName;				// Nom de la map r�pr�sentant le skin du joueur
		bool skinVisibility;		// Indique si le joueur (son skin) est affich� ou non
		bool outlineVisibility;		// Indique si les contours physiques du joueur (ellipso�de qui l'entoure) est visible
	};

	class CDebug {
	public:
		bool bSonPerformances;		// Indique si l'usage CPU du son doit �tre affich�
		bool bSonSpectre;			// Affiche le spectre sonore
		bool bAfficheFichier;		// Indique si l'ouverture des fichiers ASE doit se faire en mode debug (=>affichage dans la console durant la lecture)
		bool bReseauDebit;
		bool bAfficheNormaux;
		bool cubicMeterVisibility;	// Indique si le cube d'un m�tre de c�t� servant � calibrer les dimensions de la sc�ne est visible ou non
		bool axesMeterVisibility;	// Indique si les axes d'un m�tre de c�t� servant � calibrer les dimensions de la sc�ne sont visibles ou non
	};

	string nomFichierConfig;

public:
	CDisplay Display;				// Configuration de l'affichage
	CAudio Audio;					// Configuration audio
	CCommandes Commandes;			// Configuration des commandes
	CCentralisateur Centralisateur;	// Configuration de la connexion au centralisateur
	CReseau Reseau;					// Configuration du r�seau
	CJoueur Joueur;					// Informations sur le joueur principal (nom,...)
	CDebug Debug;					// Param�tres servant au d�buggage
		
	CCfg();

	void NommeConfig(const string &nomFichier);
	void AfficheDateCompilation();		// Affiche le n� de version du programme

	void Lit();			// Lit le fichier de configuration
	void Ecrit();		// Sauvegarde la configuration
};

#endif

