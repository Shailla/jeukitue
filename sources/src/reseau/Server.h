
#ifndef __JKT__SERVER_H
#define __JKT__SERVER_H

#include <string>

#include "reseau/new/Interlocutor2.h"
#include "reseau/new/ServerUdpInterlocutor.h"

using namespace std;

class CPlayer;

namespace JktNet
{

class CServer	// Contient les donn�es courantes du serveur (nombre de joueur, partie en cours,...)
{
	int m_uNbrPlayers;				// Nombre de joueurs actuellement dans la partie
	StatutServer m_Statut;			// Etat du serveur (d�connect�, connect�, partie en cours, ...)

public:
	int maxPlayers;					// Nombre maximum de joueurs accept�s sur le serveur
	CSPA spaMaitre;
	SDLNet_SocketSet socketSet;		// Les sockets � �couter

	string nom;						// Nom du serveur
	string nameMap;					// Nom de la MAP ouverte en mode multi-joueurs
	
	bool bGame;						// true si une partie est en cours
	string nomMAP;					// Nom du fichier MAP de la partie en cours

	// Serveur moderne
	ServerUdpInterlocutor* _serverUdpInterlocutor;

		// Constructeurs / Destructeur
	CServer();	
	~CServer();

	int AjoutePlayer(CPlayer *player);			// Ajoute un proxy-joueur � la partie en cours
	void partieEnCours(bool indic);				// Donne une valeur � bPartieEnCours
	int nbrPlayers();							// Donne le nombre de joueurs sur le serveur
	void setStatut( StatutServer statut);
	StatutServer getStatut();

	void decodeServerUDP( CSPA *spa );			// D�code les paquets ne provenant pas des joeurs actuels
	void decodeProxyPlayer( CPlayer *player ); 	// D�code les paquets provenant des diff�rents joueurs
	bool acceptPlayer( CSPA *spa );				// Accepte un nouveau joueur dans la partie

	NotConnectedInterlocutor2* connect(Uint16 port, Uint16 portTree);
	void disconnect();
	void emet();

private:
	void switchRecapServer(CPlayer *player,Uint16 code2);	
	void switchInfo(CSPA *spa,Uint16 code2);
	void switchPing(CSPA *spa,Uint16 code2);
	void switchJTG(CSPA *spa,Uint16 code2);
};

}	// JktNet

#endif

