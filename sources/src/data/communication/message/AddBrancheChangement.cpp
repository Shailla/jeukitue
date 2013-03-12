/*
 * AddBrancheChangement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <sstream>
#include <vector>

using namespace std;

#include "util/StreamUtils.h"

#include "data/communication/message/AddBrancheChangement.h"

AddBrancheChangement::AddBrancheChangement(istringstream& in) {
	unserialize(in);
}

AddBrancheChangement::AddBrancheChangement(const vector<int>& brancheId, int revision) {
	_brancheId = brancheId;
	_revision = revision;
}

void AddBrancheChangement::update(MarqueurClient* marqueur) {
	// Met � jour l'�tat des donn�es
	marqueur->setSentRevision(marqueur->getDonnee()->getRevision());
}

void AddBrancheChangement::serialize(ostringstream& out) {
	// Serialize
	out << ADD_DATA_MESSAGE;
	out << _brancheId;
	out << _revision;
}

void AddBrancheChangement::unserialize(istringstream& in) {
	in >> _brancheId;
	in >> _revision;
}

void AddBrancheChangement::change(DataTree* tree) {

}
