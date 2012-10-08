/*
 * LuaUtils.cpp
 *
 *  Created on: 11 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

namespace JktPlugin {

LuaUtils::LuaUtils() {
}

LuaUtils::~LuaUtils() {
}

/**
 * Report Lua execution errors.
 */
void LuaUtils::report_lua_errors(lua_State *L, int status) {
	if (status != 0) {
		cerr << endl << "Detail erreur Lua : " << lua_tostring(L, -1);
		lua_pop(L, 1); // remove error message
	}
}


bool LuaUtils::isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int typesNbr, ...) {
	va_list vl;
	va_start(vl, typesNbr);

	// V�rifie si le nombre de param�tres est bon
	int paramNbr = lua_gettop(L);

	if(paramNbr != typesNbr) {
		ostringstream message;
		message << "Mauvais nombre de parametres (attendu=" << typesNbr << ", recus=" << paramNbr << ")";
		PluginEngine::getPluginContext(L)->logError(message.str());

		return false;
	}

	// V�rifie les types de chaque param�tre
	for(int i=1 ; i<typesNbr+1 ; i++) {
		const LUA_PARAMETER_TYPES type = (LUA_PARAMETER_TYPES)va_arg(vl, int);

		switch(type) {
		case LUA_PARAM_STRING:
			if(!lua_isstring(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=String)";
				PluginEngine::getPluginContext(L)->logError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_BOOLEAN:
			if(!lua_isboolean(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=Booleen)";
				PluginEngine::getPluginContext(L)->logError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_NUMBER:
			if(!lua_isnumber(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=Entier)";
				PluginEngine::getPluginContext(L)->logError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_ANY:
			// Nothing to do
			break;
		default:
			cerr << endl << "Erreur Lua : Type de parametre Lua non-reconnu";
			return false;
		}
	}

	va_end(vl);

	return true;
}

} /* namespace JktPlugin */
