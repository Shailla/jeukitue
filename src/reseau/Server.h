
#ifndef __JKT__SERVER_H
#define __JKT__SERVER_H

#include "enumReseau.h"

class CPlayer;

namespace JKT_PACKAGE_RESEAU
{

class CServer	// Contient les donn�es courantes du serveur (nombre de joueur, partie en cours,...)
{
	int m_uNbrPlayers;		// Nombre de joueurs actuellement dans la partie
	StatutServer m_Statut;	// Etat du serveur (d�connect�, connect�, partie en cours, ...)

public:
	int maxPlayers;			// Nombre maximum de joueurs accept�s sur le serveur
	CSPA spaMaitre;
	SDLNet_SocketSet socketSet;		// Les sockets � �couter

	string nom;			// Nom du serveur
	string nameMap;		// Nom de la MAP ouverte en mode multi-joueurs
	
	bool bGame;		// true si une partie est en cours
	string nomMAP;	// Nom du fichier MAP de la partie en cours

		// Constructeurs / Destructeur
	CServer();	
	~CServer();

	int AjoutePlayer(CPlayer *player);	// Ajoute un proxy-joueur � la partie en cours
	int SuivantPlayer(int pos);			// Donne le joueur apr�s la position curseur
	CPlayer *GetPlayer(int pos);		// Renvoie le joueur point� par le curseur
	void partieEnCours(bool indic);		// Donne une valeur � bPartieEnCours
	int nbrPlayers();					// Donne le nombre de joueurs sur le serveur
	void setStatut( StatutServer statut);
	StatutServer getStatut();

	void decodeServerUDP( CSPA *spa );	// D�code les paquets ne provenant pas des joeurs actuels
	void decodeProxyPlayer( CPlayer *player ); // D�code les paquets provenant des diff�rents joueurs
	bool acceptPlayer( CSPA *spa );	// Accepte un nouveau joueur dans la partie

	bool ouvre( Uint16 port );
	void emet();

private:
	void switchRecapServer(CPlayer *player,Uint16 code2);	
	void switchInfo(CSPA *spa,Uint16 code2);
	void switchPing(CSPA *spa,Uint16 code2);
	void switchJTG(CSPA *spa,Uint16 code2);
};

}	// JKT_PACKAGE_RESEAU

#endif

