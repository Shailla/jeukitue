/*
 * PluginContext.cpp
 *
 *  Created on: 9 mai 2012
 *      Author: Erwin
 */

using namespace std;

#include <agar/core.h>

#include "plugin/PluginContext.h"

namespace JktPlugin {

const char* PluginContext::LOG_USER_PREFIX = 		"USER   : ";
const char* PluginContext::LOG_INFO_PREFIX = 		"INFO   : ";
const char* PluginContext::LOG_ERROR_PREFIX = 		"ERREUR : ";
const char* PluginContext::LOG_ERROR_LUA_PREFIX = 	"ERREUR LUA : ";

PluginContext::PluginContext(lua_State* luaState, const string& pluginName, const string& pluginDirectory) : _pluginName(pluginName) {
	_luaState = luaState;

	// TODO G�rer l'erreur d'ouverture du fichier de log
	string pluginLogFile = string(pluginDirectory).append(pluginName).append(".log");
	_logFile.open(pluginLogFile.c_str());
}

PluginContext::~PluginContext() {
	// Fermeture propre du fichier de logs du plugin
	_logFile.flush();
	_logFile.close();
}

lua_State* PluginContext::getLuaState() {
	return _luaState;
}

/**
 * Emet une trace demand� par le script Lua dans le fichier de log du plugin.
 */
void PluginContext::logUser(const string& trace) {
	_logFile << endl << LOG_USER_PREFIX << trace.c_str() << flush;
}

/**
 * Emet une trace d'information dans le fichier de log du plugin.
 */
void PluginContext::logInfo(const string& trace) {
	_logFile << endl << LOG_INFO_PREFIX << trace.c_str() << flush;
}

/**
 * Emet une trace d'erreur dans le fichier de log du plugin.
 */
void PluginContext::logError(const string& trace) {
	// R�cup�re le num�ro de la ligne en cours d'ex�cution dans le code LUA du plugin
	lua_Debug ar;
	lua_getstack(_luaState, 1, &ar);
	lua_getinfo(_luaState, "l", &ar);

	_logFile << endl << LOG_ERROR_PREFIX << "(ligne " << ar.currentline << ") " << trace.c_str() << flush;
}

/**
 * Si une erreur Lua s'est produite alors �met une trace dans le fichier de log du plugin.
 */
void PluginContext::logLuaError(const int status) {
	if (status != 0) {
		_logFile << endl << LOG_ERROR_LUA_PREFIX << lua_tostring(_luaState, -1) << flush;
		lua_pop(_luaState, 1);
	}
}

} /* namespace JktPlugin */
