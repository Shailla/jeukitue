/*
 * StringUtils.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <string.h>

using namespace std;

#include "ctype.h"

#include "util/StringUtils.h"

namespace JktUtils {

StringUtils::StringUtils() {
}

StringUtils::~StringUtils() {
}

/**
 * Copy a string instance in a char* and :
 *    - allocate the memory for the char*
 *    - terminate the char* with the termination character ('\0')
 */
char* StringUtils::toChars(const std::string& str) {
	char* chars = new char[str.length() + 1]();
	str.copy(chars, str.length());
	chars[str.length()] = '\0';

	return chars;
}

/**
 * Copy a string instance in a char* and :
 *    - allocate the memory for the char*
 *    - terminate the char* with the termination character ('\0')
 */
char* StringUtils::copy(const std::string& src, const char* dst) {
	char* chars = new char[src.length() + 1]();
	src.copy(chars, src.length());
	chars[src.length()] = '\0';

	return chars;
}

/**
 * Attention, cette m�thode n'est pas finie, pour l'instant elle v�rifie uniquement si la cha�ne n'est pas vide !
 * TODO Finir l'impl�mentation de la pr�sente m�thode
 * ""		: true
 * "     "	: true
 * "-"		: false
 * "  - "	: false
 */
bool StringUtils::isBlank(const char* str) {
	if(strlen(str) == 0) {
		return true;
	}

	return false;
}

} /* namespace JktUtils */
