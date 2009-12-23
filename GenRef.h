#ifndef __JKT__GENREF_H
#define __JKT__GENREF_H

/*	G�n�rateur de r�f�rences optimis�, une r�f�rence est un 'unsigned int' unique
*/

#include <list>
#include <string>

namespace JKT_PACKAGE_UTILS
{
class CGenRef
{
	class CBloc
	{
		friend CGenRef;
		unsigned int m_Debut;	// D�but du bloc
		unsigned int m_Fin;		// Fin du bloc
		CBloc(unsigned int,unsigned int);
	};
	std::list<CBloc> sequence;
public:
	CGenRef();
	~CGenRef();

	unsigned int genRef();			// G�n�re une nouvelle r�f�rence
	void delRef(unsigned int ref);	// Supprime une r�f�rence si elle existe
	std::string toString();
};
}

#endif

