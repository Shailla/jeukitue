#include "SignalementData.h"

#include "Centralisateur.h"

Centralisateur::Centralisateur(UdpController* udpController)
{
    m_udpController = udpController;
}

Centralisateur::~Centralisateur(void)
{
    delete m_udpController;
}

void Centralisateur::connecter(char* userName, char* ipCentralisateur, int portCentralisateur) 
{
    const int m_portLocal = 39455;
    m_userName = userName;
    m_udpController->connect(userName, m_portLocal, ipCentralisateur, portCentralisateur);
}

void Centralisateur::sendSignalement()
{
    m_udpController->sendSignalement(m_userName);
}