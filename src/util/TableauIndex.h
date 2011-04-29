
#ifndef __JKT__TABLEAUINDEX_H
#define __JKT__TABLEAUINDEX_H

namespace JktUtils
{

template <class X> class CTableauIndex
{
private:
	int m_Max;		// Nombre de places dans ce tableau
	int m_Nbr;		// Nombre d'�l�ments actuellement pr�sents dans le tableau
	X** m_XTableau;	// Tableau des objets
	bool m_bParent;	// Indique si les �l�ments du tableau doivent �tre d�truits en m�me temps que lui

		// Constructeurs/destructeur
public:
	CTableauIndex(int nbr, bool bParent);	// Cr�e un tableau index� pouvant contenir nbr objets
	~CTableauIndex();				// D�truit le tableau (ne d�truis pas les �l�ments par contre)

	int getMax();					// Retourne le nombre de places disponibles dans le tableau
	int getNbr();					// Retourne le nombre d'�l�ments actuellement pr�sents dans le tableau
	bool Ajoute(int pos,X *objet);	// Ajoute un �l�ment � la pos� position du tableau
	int Ajoute( X *objet );			// Ajoute l'�l�ment � la premi�re place de libre
	bool Supprime(int pos);			// Supprime du tableau le pos� �l�ment
	X* operator[](int pos);			// Retourne un pointeur sur l'�l�ment index� 'pos'
	int Suivant( int pos );			// Renvoie l'index de l'�l�ment apr�s l'�l�ment index� par 'pos'
	bool bSuivant( int &pos );		// Comme Suivant + document pos + v�rifie si dernier �l�ment
};

}	// JktUtils

#endif

