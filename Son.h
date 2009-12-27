
#ifndef __JKT__SON_H
#define __JKT__SON_H

#include <map>
#include <set>
#include <string>

using namespace std;

namespace JKT_PACKAGE_SON {

class CReqSon;
class CDemonSons;

class CSon
{
public:
	FSOUND_SAMPLE *m_Sample;	// Echantillons du son � proprement dit
	CSon* m_IdSon;				// Identifiant de cette classe CSon dans le d�mon
	set<CReqSon*> m_TabReq;		// Liste des requ�tes sur ce son
	CDemonSons* pDemon;			// Pointeur sur le d�mon des sons
	string nom;					// Nom du son (POUR LE DEBUG)

		// Constructeurs / Destructeur
public:
	CSon(CDemonSons* p,const char* name);
	virtual ~CSon();

	virtual CReqSon *PlayID( bool pause=false) = 0;
};

class CSonMono : public CSon
{
public:
		// Constructeurs / Destructeur
	CSonMono(CDemonSons* p,const char* nomFichierSon);
	~CSonMono();

	CReqSon *PlayID(bool pause=false);
};

class CSonStereo : public CSon
{
public:
		// Constructeurs / Destructeur
	CSonStereo(CDemonSons* p,const char* nomFichierSon);
	~CSonStereo();

	CReqSon *PlayID( bool pause=false );
};

class CSon3D : public CSon
{
public:
		// Constructeurs / Destructeur
	CSon3D(CDemonSons* p,const char* nomFichierSon);
	~CSon3D();

	CReqSon *PlayID( bool pause=false );
};

}	// namespace JKT_PACKAGE_SON
#endif
