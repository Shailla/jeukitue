/*
 * AddBrancheChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

using namespace std;

#include "data/communication/message/AddBrancheChangement.h"

AddBrancheChangement::AddBrancheChangement(IMessageStream& in) {
	unserialize(in);
}

AddBrancheChangement::AddBrancheChangement(vector<int>& brancheId, int revision) {
	_brancheId = brancheId;
	_revision = revision;
}

void AddBrancheChangement::update(MarqueurClient* marqueur) {
	// Met � jour l'�tat des donn�es
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheChangement::serialize(OMessageStream& out) {
	// S�rialise
	out << ADD_DATA_MESSAGE;
	out << _brancheId;
	out << _revision;
}

void AddBrancheChangement::unserialize(IMessageStream& in) {
	in >> _brancheId;
	in >> _revision;
}

void AddBrancheChangement::change(DataTree* tree) {

}
