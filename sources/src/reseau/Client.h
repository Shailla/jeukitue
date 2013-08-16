

#ifndef __JKT__CLIENT_H
#define __JKT__CLIENT_H

#include "SDL.h"
#include "SDL_net.h"
#include "reseau/enumReseau.h"
#include "reseau/SPA.h"
#include "reseau/new/ClientUdpInterlocutor.h"

class CPlayer;

namespace JktNet
{

class CClient {
public:
	class CInfoServer {
		bool m_bReady;						// Informe si ces infos ont �t� actualis�es depuis la derni�re requ�te d'infos
	public:
		string nom;						// Nom du serveur
		string map;						// Nom de la MAP jou�e sur le serveur
		CInfoServer();
		CInfoServer &operator=( CInfoServer &infoServer );
		void Ready( bool ready );
		bool Ready();
	};

private:
	Uint32 m_timePingClientServer;	// Temps � l'envoie d'un ping du client vers le serveur
	int m_pingClientServer;			// Temps qu'� mis le dernier ping client->serveur

	CInfoServer m_InfoServer;		// Map en cours, nom du serveur, ...

	int m_uNbrPlayers;				// Nombre de joueurs sur le serveur
	int m_MaxPlayers;				// Nombre maximum de joueurs sur le serveur

	// Serveur moderne
	ClientUdpInterlocutor* _clientUdpInterlocutor;

public:
	SDLNet_SocketSet socketSet;		// Les sockets � �couter
	CSPA spaMaitre;
	StatutClient m_Statut;			// Etat du client (demande de connexion envoy�e, connect�, ...)
	int IDpersonnel;				// Identifiant du joueur (sur ce PC)
	string nomMAP;					// Nom de la MAP en cours

		// Constructeurs/destructeurs
	CClient();
	~CClient();

	void sendRequestInfoToServer();
	void sendPingToServer();
	void sendJoinTheGame( string &nomPlayer );

	void recoit();

	void setStatut(StatutClient statut);		// Renseigne l'�tat du client (d�connect�,connect�, partie en cours...)
	StatutClient getStatut();					// Retourne l'�tat du client

	void setMaxPlayers(int nbr);				// Cr�e le tableau des joueurs et enregistre le nombre de joueurs maxi
	int getMaxPlayers();						// Retourne le nombre maximum de joueurs

	unsigned int nbrPlayers();					// Donne le nombre de joueurs sur la MAP en cours
	void nbrPlayers(unsigned int nbr);			// Impl�mente le nombre de joueurs sur la MAP en cours
	bool AjoutePlayer(int pos,CPlayer *player);	// Ajoute un joueur dans la liste des joueurs
	CPlayer* GetPlayer(int pos);				// Retourne un pointeur sur l'�l�ment index� 'pos'
	int SuivantPlayer( int pos );				// Renvoie l'index de l'�l�ment apr�s l'�l�ment index� par 'pos'

	void decodeConnecte( Uint16, Uint16 ); 		//D�code les paquets recus du r�seau
	bool decodeNonConnecte( Uint16 code1, Uint16 code2 );
	Interlocutor2* ouvre(const string &address, Uint16 port, Uint16 portTree);
	void ferme();
	CInfoServer getInfoServer();
	int getPingClientServeur();
	void emet( CPlayer &player );

private:
	void decodeRecap( Uint16 code2 );		// Gestion des msg de r�capitulation de partie
	void decodeError( Uint16 code2 );		// Gestion des msg d'erreur
};

}	// JktNet

#endif

