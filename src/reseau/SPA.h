
#ifndef __JKT__SPA_H
#define __JKT__SPA_H

#include <iostream>
#include <string>
using namespace std;

#include "SDL.h"
#include "SDL_net.h"

class CPlayer;

namespace JktNet
{

class CSPA
{
private: 
	static int m_BytesRec;	// Nombre d'octets pour le comptage du d�bit en r�ception
	static Uint32 m_TimeBytesRec;
	static float m_fDebitRec;
	static float m_fTailleRec;
	static int m_NombreRec;

	static int m_BytesEm;	// Nombre d'octets pour le comptage du d�bit en �mmission
	static Uint32 m_TimeBytesEm;
	static float m_fDebitEm;
	static float m_fTailleEm;
	static int m_NombreEm;

	unsigned int m_uPosition;		// Position en octets dans le paquet
	UDPpacket *m_PacketIn;			// Paquet recevant les donn�es
	UDPpacket *m_PacketOut;			// Paquet pour l'envoie des donn�es
	IPaddress m_Ip;					// Adresse du distant
	UDPsocket m_Socket;				// Ben �a c'est la socket :)

public:
	CSPA();
	~CSPA();

	bool open( const IPaddress &address );				// Ouverture en mode client
	bool open( const string &remIp, Uint16 remPort );	// Ouverture en mode client
	bool open( Uint16 locPort );						// Ouverture en mode serveur
	void close();

	void init();
	UDPpacket *getPacketIn() const;
	UDPpacket *getPacketOut() const;
	UDPsocket getSocket() const;

	unsigned int getPosition() const;		// Retourne la position dans le paquet
	
	void add16InPosition( unsigned int position, Uint16 var );
	
	void addCode( Uint16 code1, Uint16 code2 );		// Ajoute un doublet de codes � la fin du paquet
	void addBool( bool var );						// Ajoute un bool�en � la fin du paquet
	void add16( Uint16 var );						// Ajoute un mot de 16 bits � la fin du paquet
	void add32( Uint32 var );						// Ajoute un mot de 32 bits � la fin du paquet
	void add( float var);							// Ajoute un tableau de 3 flottants
	void add3fv( float *var);						// Ajoute un tableau de 3 flottants
	void addChar( const char *txt );				// Ajoute du texte � la fin du paquet
	void add( const string &txt );					// Ajoute du texte � la fin du paquet
	void addRecapFromServer( const CPlayer &player );

	void readCode(Uint16 &code1,Uint16 &code2);		// Lit le doublet de code point�
	bool readBool();								// Lit le bool�en point�
	Uint16 read16();								// Lit le mot de 16 bits point�
	Uint32 read32();								// Lit le mot de 32 bits point�
	float readf();									// Lit un tableau de 3 flottants
	void read3fv( float *var);						// Lit un tableau de 3 flottants
	void readChar( char *txt );						// Lit le texte point�
	void readString( string &mot );					// Lit le texte point� vers un string
	void readRecapFromServer( const CPlayer &player );

	string iPInToString() const;					// Renvoie l'IP in en string
	string iPOutToString() const;					// Renvoie l'IP out en string
	string debugToString();							// Renvoie un r�sum� du paquet sous forme de string
	int send();										// Envoie le CSPA depuis packetOut
	int recoit();									// Recoit les paquets du CSPA->packetIn
	static void getDebitRec( float &debit, float &taille );	// Calcul le d�bit en r�ception depuis le dernier calcul de celui-ci
	static void getDebitEm( float &debit, float &taille );	// Calcul le d�bit en �mmission depuis le dernier calcul de celui-ci
};

}	// JktNet

#endif

