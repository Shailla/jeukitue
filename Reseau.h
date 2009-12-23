
#ifndef __JKT__CRESEAU_H
#define __JKT__CRESEAU_H

#include "enumReseau.h"

/* Remarque, le protocole est comme suit par exemple :
	- UDP_INFO, NULL : demande les infos serveur
	- UDP_ACK, UDP_INFO  : acquitte et transmet les infos du serveur
	- UDP_ERROR, code d'erreur, "texte explicatif" : informe d'une erreur
	- UDP_PING, NULL : Demande un ping
	- UDP_PING, UDP_ACK : R�ponse � un ping
*/

namespace JKT_PACKAGE_RESEAU
{

class CReseau
{
		// Variables membres 
	CServer *m_Server;
	CClient *m_Client;

	bool m_On;						// Indique si le r�seau peut �tre utilis� ou non

public:

		// Constructeurs / Destructeur
	CReseau();		
	~CReseau();

	void setOn( bool on );					// Indique si le r�seau peut �tre utilis� ou non
	bool getOn();							// Est-ce que le r�seau peut �tre utilis� ou non ?

	CServer *getServer();
	CClient *getClient();

	bool ouvreServer( Uint16 port );		// Ouvre le serveur
	void fermeServer();						// Ferme le serveur
	
	bool ouvreClient();						// Connecte le client au serveur
	void fermeClient();						// D�connecte le client du serveur
	void setStatutClient( StatutClient statut );
	StatutClient getStatutClient() const;

	void recoitClient();					// Recoit tous les paquets
	void recoitServer();					// Recoit tous les paquets

	void sendJoinTheGame();					// Joins la partie en cours sur le serveur
	void sendPingClientServer();			// Envoie d'un ping du client vers le serveur
	int getPingClientServer();				// R�cup�re la valeur du dernier ping puis efface celle-ci
	void sendRequestInfoServer();			// Demande ses infos au serveur

	CClient::CInfoServer getInfoServer();	// R�cup�re les dervi�res info obtenues du serveur
};

}	// JKT_PACKAGE_RESEAU

#endif

