#ifndef __JKT__OPENUDPSOCKETEXCEPTION_H
#define __JKT__OPENUDPSOCKETEXCEPTION_H

#include "exception/centralisateur/CentralisateurException.h"

/**
    A utiliser pour indiquer un �chec d'ouverture de socket UDP
*/
class OpenUdpSocketException : public CentralisateurException
{
    int m_port;
    const char* m_message;
public:
    OpenUdpSocketException(const int port, const char* message);
    ~OpenUdpSocketException(void);
};

#endif
