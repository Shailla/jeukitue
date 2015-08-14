/*
 * Donnee.h
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#ifndef DONNEE_H_
#define DONNEE_H_

#include <map>

class DistantTreeProxy;
class MarqueurDistant;
class Branche;
class Valeur;

/**
 * Indique de quelle mani�re la donn�es est partag�e entre le serveur et les clients.
 * Si la donn�e est une branche alors tous ses fils h�riteront de son type.
 *
 * DONNEE_SERVER : Donn�e partag�e par le serveur avec tous les clients
 * DONNEE_CLIENT : Donn�e partag�e par le serveur avec chacun des clients (chaque client poss�de ses propres donn�es)
 * DONNEE_LOCAL  : Donn�e non-partag�e, elle reste en local sur le serveur ou le client
 */
enum DONNEE_TYPE {
	DONNEE_DEFAULT,		// On peut cr�er une donn�e de type
	DONNEE_SERVER,		// Donn�e partag�e par le serveur avec tous les clients
	DONNEE_CLIENT,		// Donn�e partag�e par le serveur avec chacun des clients (chaque client poss�de ses propres donn�es)
	DONNEE_CLIENT_SUB,	// Donn�e partag�e par le serveur avec chacun des clients (chaque client poss�de ses propres donn�es)
	DONNEE_LOCAL		// Donn�e non-partag�e, elle reste en local sur le serveur ou le client
};

class Donnee {
	/**
	 * Revision number of the data, incremented each time the data changes.
	 */
	int _revision;
	std::map<DistantTreeProxy*, MarqueurDistant*> _marqueurs;
	DONNEE_TYPE _donneeType;

protected:

	/**
	 * Indicates that the Donnee has changed by incrementing it's revision.
	 */
	void update();

public:
	Donnee(int revision, DONNEE_TYPE donneeType);
	virtual ~Donnee();

	MarqueurDistant* getMarqueur(DistantTreeProxy* distant);
	MarqueurDistant* addMarqueur(DistantTreeProxy* distant, int donneeTmpId);

	DONNEE_TYPE getDonneeType() const;

	/**
	 * Get the revision number of the data.
	 */
	int getRevision() const;
	void setRevision(int revision);

	static bool highestRevision(const Donnee* left, const Donnee* right);
};

#endif /* DONNEE_H_ */
