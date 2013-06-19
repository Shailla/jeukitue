/*
 * UdpInterlocutor.cpp
 *
 *  Created on: 18 juin 2013
 *      Author: vgdj7997
 */

#include "reseau/new/UdpInterlocutor.h"

UdpInterlocutor::UdpInterlocutor() {
}

UdpInterlocutor::~UdpInterlocutor() {
}

void UdpInterlocutor::connect() {
	// Obtenir port de r�ponse UDP
	// Envoyer au distant demande connexion et fournir port de r�ponse
	// Obtenir acceptation connexion de la part du client

	setConnexionStatus(CONNEXION_STATUS::CONNECTED);
}

void UdpInterlocutor::close() {
	// Envoyer message d�connexion au distant
	// Passer statut TechnicalInterlocutor � D�connect�
	// Lib�rer les ressources (sockets, ...)
}

