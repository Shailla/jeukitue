
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <fstream>
#include <map>
#include <iostream>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/Trace.h"
#include "util/TraceMethod.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "util/TableauIndex.cpp"
#include "spatial/IfstreamMap.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/Clavier.h"
#include "spatial/contact.h"
#include "main/Game.h"

#include "reseau/enumReseau.h"
#include "reseau/Server.h"

void JktMoteur::contactPlayer(CPlayer *player, float *normal, float distanceW);
void gravitePlayer(CPlayer *player);

extern CGame Game;
class CMap;

namespace JktNet
{

CServer::CServer()
{
TRACE().debug("CServer::CServer() begin%T", this );
	maxPlayers = 10;					// 10 joueurs maxi pas d�faut
	m_uNbrPlayers = 0;					// Aucun joueur
	nom = "ServerJKT";					// Nom de serveur par d�faut
	bGame = false;						// Pas de partie en cours
	nomMAP = "";						//donc pas de nom de partie
	m_Statut = JKT_STATUT_SERVER_INIT;	// Indique que le serveur est en cours d'initialisation

	socketSet = 0;

	_serverUdpInterlocutor = 0;

TRACE().debug("CServer::CServer() end%T", this );
}

CServer::~CServer() {
}

void CServer::disconnect() {
TRACE().debug("CServer::~CServer() begin%T", this );
	SDLNet_FreeSocketSet( socketSet );		// Lib�re le socket set du serveur
	socketSet = 0;

	if(_serverUdpInterlocutor)
		_serverUdpInterlocutor->close();

	delete _serverUdpInterlocutor;
TRACE().debug("CServer::~CServer() end%T", this );
}

CPlayer *CServer::GetPlayer( int pos )
{	return Game._pTabIndexPlayer->operator []( pos );	}

void CServer::setStatut( StatutServer statut )
{	m_Statut = statut;		}

StatutServer CServer::getStatut()
{	return m_Statut;		}

int CServer::AjoutePlayer( CPlayer *player ) {
TRACE().debug("CServer::AjoutePlayer(player=%x)%T", player, this );

	int nbr;
	if( m_uNbrPlayers<this->maxPlayers ) {
		m_uNbrPlayers++;		// Incr�mente le nbr de joueurs sur le serveur
		nbr = Game._pTabIndexPlayer->Ajoute( player );	// Ajoute le joueur � la liste
	}
	else {
		nbr = -1;
	}

TRACE().debug("CServer::AjoutePlayer() -> %d end%T", nbr, this );
	return nbr;
}

int CServer::nbrPlayers()
{	return m_uNbrPlayers;	}


void CServer::decodeServerUDP( CSPA *spa ) {
	Uint16 code1, code2;
	if( spa->recoit() )			// Si des paquets ont �t� re�us
	{
		spa->init();
		spa->readCode( code1, code2 );
		switch( code1 )
		{
		case CLIENT_INFO:		// Un client demande les info serveur
			switchInfo( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

   		case CLIENT_PING:	// Un client a envoy� un ping
			switchPing( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		case CLIENT_JTG:		// Un client demande � entrer dans le jeu en cours sur le serveur
			switchJTG( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		default:
			TRACE().error("Paquet inconnu decodeServerUDP (%d)", code1);
			break;
		}
	}
}

void CServer::decodeProxyPlayer( CPlayer *player ) {
	Uint16 code1, code2;

	if( player->spa.recoit() ) {	// Si des paquets ont �t� re�us
		player->spa.init();
		player->spa.readCode( code1, code2 );
		switch( code1 ) {
		case CLIENT_RECAP:		// R�ception d'une r�capitulation de partie
			switchRecapServer( player, code2 );
			break;

		default:
			TRACE().warn("Reception d'un paquet PROXYPLAYER inconnu");
			break;
		}
	}
}

bool CServer::acceptPlayer( CSPA *spa ) {
	TRACEMETHOD();
	bool result = true;
	char nomNewPlayer[50];
	CPlayer *player, *player2;

	player = new CPlayer();								// Cr�e le r�ceptacle du nouveau joueur
	player->changeAction( gravitePlayer );				// Associe au joueur une fonction de gravit�
	player->changeContact( JktMoteur::contactPlayer );	// Associe une fonction de gestion des contacts avec la map
	spa->readChar( nomNewPlayer );						// R�cup�re le nom du nouveau joueur
	player->nom( nomNewPlayer );						// Enregistre le nom du nouveau joueur
	player->init();

	// Ouverture d'un lien SPA et cr�ation du joueur
	result = player->spa.openInClientMode( spa->getPacketIn()->address );

	if( result ) {
		if( SDLNet_UDP_AddSocket( socketSet, player->spa.getSocket() )==-1 ) {
TRACE().debug("CServer::acceptPlayer() : %s%T", SDLNet_GetError(), this );
			TRACE().error("SDLNet_AddSocket : ", SDLNet_GetError());

			player->spa.close();

			delete player;

			result = false;
		}
	}

	int IDPlayer = AjoutePlayer( player );	// Ajoute le joueur pour de bon et identifie-le
	Game.Erwin( player );					// Place le nouveau joueur comme joueur principal


	/* /////////////////////////////////////////////////////////////////////////////////
	// Dis au client qu'il est accept� dans le jeu et donne-lui les infos de la partie
	////////////////////////////////////////////////////////////////////////////////////*/

	player->spa.init();
	player->spa.addCode( SERVER_JTG, SERVER_ACK );		// Acquitte la jonction du joueur � la partie
	player->spa.add16( (Uint16)IDPlayer );				// Envoie son identifiant au nouveau proxy-joueur
	player->spa.add( nomMAP );							// Envoie le nom de la MAP en cours
	player->spa.add16( (Uint16)maxPlayers );			// Envoie le nombre max de joueurs sur la MAP
	player->spa.add16( (Uint16)this->m_uNbrPlayers );	// Envoie le nombre de joueurs en cours

	// Envoie les infos concernant chaque joueur
	int curseur = -1;

	while(Game._pTabIndexPlayer->Suivant(curseur)) {
		player2 = GetPlayer( curseur );

		player->spa.add16( (Uint16)curseur );			// Envoie l'identifiant de ce proxy-joueur
		player->spa.add( player2->nom() );	// Envoie son nom

		player->spa.addRecapFromServer( *player2 );
	}

	player->spa.send();

	return result;	// OK
}

void CServer::switchRecapServer( CPlayer *player, Uint16 code2 ) {
	Uint16 flags;

	switch( code2 ) {
	case CLIENT_NULL:
		flags = player->spa.read16();

		player->getClavier()->m_fAvance = 0.0f;
		player->getClavier()->m_fDroite = 0.0f;
		player->getClavier()->m_fMonte = 0.0f;

		if( flags&0x0001 ) {	// S'il y a des requ�tes
			player->getClavier()->m_bIndic = true;	// Indique la pr�sence d'une requ�te

			if( flags&0x0010 ) {	// Y a-t-il une requ�te de mouvement en avant
				player->getClavier()->m_fAvance = player->spa.readf();
			}

			if( flags&0x0100 ) {	// Y a-t-il une requ�te de mouvement � droite
				player->getClavier()->m_fDroite = player->spa.readf();
			}

			if( flags&0x1000 ) {	// Y a-t-il une requ�te de mouvement vers le haut
				player->getClavier()->m_fMonte = player->spa.readf();
			}
		}
		else {
			player->getClavier()->m_bIndic = false;
		}

		player->Phi( player->spa.readf() );
		player->Teta( player->spa.readf() );

		break;

	default:
		TRACE().error("Reception d'un paquet RECAP inconnu");
		break;
	}
}

void CServer::switchInfo( CSPA *spa, Uint16 code2 ) {	// R�ception d'une demande d'info serveur
TRACE().debug("CServer::switchInfo(spa=%x,code2=%d) begin%T", spa, code2, this );
	switch( code2 ) {
	case CLIENT_NULL:
		TRACE().info("Server says : Reception d'une demande d'info serveur");
		spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );

		if( spa->getPacketOut()->channel==-1 ) {
			TRACE().error("SDLNet_UDP_Bind : ", SDLNet_GetError());
			TRACE().error("Reponse info serveur impossible");
		}
		else {
			// Envoie des infos serveur au client
			TRACE().info("Server says : Emmission des infor serveur");

			spa->init();
			spa->addCode( SERVER_INFO, SERVER_ACK );
			spa->add( nom );
			spa->add( nomMAP );
			spa->send();
		}
		break;

	default:
TRACE().debug("CServer::switchInfo() : Packet inconnu%T", this );
		TRACE().info("Reception d'un paquet inconnu");
		break;
	}
TRACE().debug("CServer::switchInfo() end%T", this );
}

void CServer::switchPing( CSPA *spa, Uint16 code2 )	// R�ception d'une demande de ping
{
TRACE().debug("CServer::switchPing(spa=%x,code2=%d) begin%T", spa, code2, this );
	switch( code2 ) {
	case CLIENT_NULL:
		if( spa->getPacketIn()->len==8 ) {
			TRACE().info("Server says : Reception d'une demande de ping");
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );

			if( spa->getPacketOut()->channel==-1 ) {
				TRACE().error("SDLNet_UDP_Bind : %s", SDLNet_GetError());
				TRACE().error("Reponse ping impossible");
			}
			else {
TRACE().debug("CServer::switchPing() : Envoi reponse ping%T", this );
					// Envoie de la r�ponse au ping
				Uint32 time;
				TRACE().info("Server says : Emmission de la reponse au ping");
				time = spa->read32();

				spa->init();
				spa->addCode( SERVER_PING, SERVER_ACK );
				spa->add32( time );
				spa->send();
			}
		}
		break;

	default:
		TRACE().warn("Reception d'un paquet inconnu");
		break;
	}
TRACE().debug("CServer::switchPing() end%T", this );
}

void CServer::switchJTG( CSPA *spa, Uint16 code2 )	// R�ception d'une demande de JTG
{
TRACE().debug("CServer::switchJTG(spa=%x,code2=%d) begin%T", spa, code2, this );
	switch( code2 )
	{
	case CLIENT_NULL:
		TRACE().info("Server says : Reception de demande a se joindre a la partie");
		if( m_Statut!=JKT_STATUT_SERVER_PLAY )	// S'il n'y a pas de partie en cours
		{
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );

			if( spa->getPacketOut()->channel==-1 ) {
TRACE().debug("CServer::switchJTG() SDLNet_UDP_Bind : %s%T", SDLNet_GetError(), this );
				TRACE().error("SDLNet_UDP_Bind : %s", SDLNet_GetError());
				TRACE().error("Reponse ping impossible");
			}
			else {
TRACE().debug("CServer::switchJTG() : Pas de partie en cours%T", this );
					// Envoie d'une erreur signalant qu'il n'y a pas de partie en cours
				TRACE().info("Demande refusee, pas de partie en cours");
				spa->init();
				spa->addCode( SERVER_JTG, SERVER_ERROR );
				spa->add16( UDP_ERROR_NOGAME );
				spa->send();
				SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			}
			break;
		}
		if( m_uNbrPlayers>=maxPlayers )	// Si le nbr max de joueurs est atteint
		{
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );
			if( spa->getPacketOut()->channel==-1 ) {
TRACE().debug("CServer::switchJTG() SDLNet_UDP_Bind : %s%T", SDLNet_GetError(), this );
				TRACE().error("SDLNet_UDP_Bind : %s", SDLNet_GetError());
				TRACE().error("Reponse ping impossible");
			}
			else {
TRACE().debug("CServer::switchJTG() : Serveur sature%T", this );
				TRACE().info("Serveur satur�");
				spa->init();
				spa->addCode( SERVER_JTG, SERVER_ERROR );
				spa->add16( UDP_ERROR_SATURE );
				spa->send();
				SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			}
			break;
		}

		if( acceptPlayer( spa ) )		// Ajoute le client associ� � spa en tant que joueur
			TRACE().info("Demande accept�e");
		else
			TRACE().error(" Demande infructueuse");
		break;
	default:
TRACE().debug("CServer::switchJTG() : Packet inconnu%T", this );
		TRACE().error("Reception d'un paquet inconnu 3");
		break;
	}
TRACE().debug("CServer::switchJTG() end%T", this );
}

NotConnectedInterlocutor2* CServer::connect(Uint16 port, Uint16 portTree) {

	/* ***********************************************************************
	 * Ouverture d'un serveur primitif
	 * **********************************************************************/

	bool result = true;
	NotConnectedInterlocutor2* notConnectedServerInterlocutor = 0;
	TRACE().info("Ouverture d'un serveur sur port %d et portTree %d", port, portTree);

	if( !spaMaitre.openInServerMode( port ) ) {
TRACE().debug("CServer::ouvre() : %s %T", SDLNet_GetError(), this );
		result = false;
	}

	if(result) {
		socketSet = SDLNet_AllocSocketSet( 20 );	// Nombre maxi de sockets � �couter

		if( !socketSet ) {
TRACE().debug("CServer::ouvre() : %s %T", SDLNet_GetError(), this );
			TRACE().error("SDLNet_AllocSocketSet : %s", SDLNet_GetError());

			spaMaitre.close();

			result = false;
		}
	}

	if(result) {
		if( SDLNet_UDP_AddSocket( socketSet, spaMaitre.getSocket() )==-1 ) {
TRACE().debug("CServer::ouvre() : %s %T", SDLNet_GetError(), this );
			disconnect();

			result = false;
		}
	}


	/* ***********************************************************************
	 * Ouverture d'un serveur moderne
	 * **********************************************************************/

	if(result) {
		_serverUdpInterlocutor = new ServerUdpInterlocutor("Server", portTree);
		notConnectedServerInterlocutor = _serverUdpInterlocutor->connect();
	}

TRACE().debug("CServer::ouvre() -> %b end%T", result, this);
	return notConnectedServerInterlocutor;	// Ouverture serveur r�ussie
}

void CServer::emet() {
	CPlayer *player, *player2;
	int curseur = -1, curseur2;

	while(Game._pTabIndexPlayer->Suivant(curseur)) {	// Envoie � chaque client-joueur
		player = GetPlayer( curseur );

		if(player) {
			player->spa.init();
			player->spa.addCode( SERVER_RECAP, SERVER_NULL );		// Code de r�capitulation
			player->spa.add16( (Uint16)nbrPlayers() );				// Envoie le nombre de joueur

				// Envoie les infos concernant chaque joueur
			curseur2 = -1;

			while(Game._pTabIndexPlayer->Suivant(curseur2)) {
				player2 = GetPlayer( curseur2 );

				if(player2) {
					player->spa.add16( (Uint16)curseur2 );			// Envoie l'identifiant de ce proxy-joueur
					player->spa.addRecapFromServer( *player2 );
				}
			}

			player->spa.send();
		}
	}
}

}	// JTK_PACKAGE_RESEAU
