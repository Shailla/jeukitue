/*
 * PluginConfigurationProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Cfg.h"

#include "plugin/lua/proxy/cfg/PluginConfigurationProxy.h"

extern CCfg Config;		// Contient la configuration du jeu

namespace JktPlugin {

/**
 * Obtenir les dimensions de l'�cran.
 *    - Return 1 : Dimension horizontale en pixels
 *    - Return 2 : Dimension verticale en pixels
 */
int PluginConfigurationProxy::getScreenSize(lua_State *L) {
	LuaUtils::checkLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_pushnumber(L, 123);
	lua_pushnumber(L, 123);

	return 2;
}

/**
 * Indique si le skin du joueur est visible ou au contraire non affich�.
 *    - Return 1 : boolean, true si skin joueur affich�, false sinon
 */
int PluginConfigurationProxy::isPlayerSkinVisible(lua_State* L) {
	LuaUtils::checkLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_pushboolean(L, Config.Joueur.skinVisibility);

	return 1;
}

/**
 * Rendre le skin du joueur visible ou au contraire non affich�.
 *    - Param 1 : boolean, true si skin joueur affich�, false sinon
 */
int PluginConfigurationProxy::setPlayerSkinVisibility(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_BOOLEAN)) {
		bool isPlayerSkinVisible = lua_toboolean(L, 1);
		Config.Joueur.skinVisibility = isPlayerSkinVisible;
	}

	return 0;
}

/**
 * Indique si le contour solide du joueur est visible ou au contraire non affich�.
 *    - Return 1 : boolean, true si le contour est affich�, false sinon
 */
int PluginConfigurationProxy::isPlayerOutlineVisible(lua_State* L) {
	LuaUtils::checkLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_pushboolean(L, Config.Joueur.outlineVisibility);

	return 1;
}

/**
 * Rendre le contour physique du joueur visible ou au contraire non affich�.
 *    - Param 1 : boolean, true si le contour du joueur est affich�, false sinon
 */
int PluginConfigurationProxy::setPlayerOutlineVisibility(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_BOOLEAN)) {
		bool isPlayerOutlineVisible = lua_toboolean(L, 1);
		Config.Joueur.outlineVisibility = isPlayerOutlineVisible;
	}

	return 0;
}

/**
 * Indique si un cube d'1 m�tre de c�t� est affich�, il sert � calibrer les dimensions dans la sc�ne.
 *    - Return 1 : boolean, true le cube d'un m�tre est affich�, false sinon
 */
int PluginConfigurationProxy::isCubicMeterVisible(lua_State* L) {
	LuaUtils::checkLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_pushboolean(L, Config.Joueur.cubicMeterVisibility);

	return 1;
}

/**
 * Afficher ou non un cube d'1 m�tre de c�t� est affich�, il sert � calibrer les dimensions dans la sc�ne.
 *    - Param 1 : boolean, true si un cube d'1 m�tre de c�t� est affich�, false sinon
 */
int PluginConfigurationProxy::setCubicMeterVisibility(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_BOOLEAN)) {
		bool cubicMeterVisibility = lua_toboolean(L, 1);
		Config.Joueur.cubicMeterVisibility = cubicMeterVisibility;
	}

	return 0;
}

/**
 * Indique si les axes d'1 m�tre de c�t� sont affich�s, ils servent � calibrer les dimensions dans la sc�ne.
 *    - Return 1 : boolean, true si les axes d'1 m�tre de c�t� sont affich�s, false sinon
 */
int PluginConfigurationProxy::isAxesMeterVisible(lua_State* L) {
	LuaUtils::checkLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_pushboolean(L, Config.Joueur.axesMeterVisibility);

	return 1;
}

/**
 * Afficher ou non les axes d'1 m�tre de c�t� sont affich�s, ils servent � calibrer les dimensions dans la sc�ne.
 *    - Param 1 : boolean, true si les axes d'1 m�tre de c�t� sont affich�s, false sinon
 */
int PluginConfigurationProxy::setAxesMeterVisibility(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_BOOLEAN)) {
		bool axesMeterVisibility = lua_toboolean(L, 1);
		Config.Joueur.axesMeterVisibility = axesMeterVisibility;
	}

	return 0;
}

} /* namespace JktPlugin */
