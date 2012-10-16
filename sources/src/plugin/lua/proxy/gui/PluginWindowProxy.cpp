/*
 * PluginWindowProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/gui/PluginWindowProxy.h"

namespace JktPlugin {

const char PluginWindowProxy::className[] = "Window";

Lunar<PluginWindowProxy>::RegType PluginWindowProxy::methods[] = {
		{"setTitle", &PluginWindowProxy::setTitle},
		{"show", &PluginWindowProxy::show},
		{"hide", &PluginWindowProxy::hide},
		{"setSize", &PluginWindowProxy::setSize},
		{"setPosition", &PluginWindowProxy::setPosition},

		{"addCheckbox", &AbstractPluginPanelProxy::addCheckbox},
		{"addNotebook", &AbstractPluginPanelProxy::addNotebook},
		{"addBoxHoriz", &AbstractPluginPanelProxy::addBoxHoriz},
		{"addBoxVert", &AbstractPluginPanelProxy::addBoxVert},
		{"addNumeric", &AbstractPluginPanelProxy::addNumeric},
		{0, 0}
};

PluginWindowProxy::PluginWindowProxy(lua_State* L) : AbstractPluginPanelProxy(L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	_pluginContext = NULL;

	_pluginWindow = new PluginWindow(PluginEngine::getPluginContext(L));
	AbstractPluginPanelProxy::setPluginContext(PluginEngine::getPluginContext(L));
	AbstractPluginPanelProxy::setPanel((PluginPanel*)_pluginWindow);
}

PluginWindowProxy::~PluginWindowProxy() {
//	if(_pluginWindow) {
//		delete _pluginWindow;
//	}
}

/**
 * Modifier les dimensions de la fen�tre.
 *    - Param 1 : Position du coin haut gauche sur l'axe horizontal en pixels
 *    - Param 2 : Position du coin haut gauche sur l'axe vertical en pixels
 */
int PluginWindowProxy::setPosition(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		const int x = lua_tonumber(L, 1);
		const int y = lua_tonumber(L, 2);
		_pluginWindow->setPosition(x, y);
	}

	return 0;
}

/**
 * Modifier les dimensions de la fen�tre.
 *    - Param 1 : Taille sur l'axe horizontal en pixels
 *    - Param 2 : Taille sur l'axe vertical en pixels
 */
int PluginWindowProxy::setSize(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		const int w = lua_tonumber(L, 1);
		const int h = lua_tonumber(L, 2);
		_pluginWindow->setSize(w, h);
	}

	return 0;
}

/**
 * Modifier le titre de la fen�tre.
 *    - Param 1 : Titre de la fen�tre
 */
int PluginWindowProxy::setTitle(lua_State* L) {
	lua_gettop(L);

	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const char* title = lua_tostring(L, 1);
		_pluginWindow->setTitle(title);
	}

	return 0;
}

/**
 * Afficher la fen�tre.
 */
int PluginWindowProxy::show(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginWindow->show();
	}

	return 0;
}

/**
 * Afficher la fen�tre.
 */
int PluginWindowProxy::hide(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginWindow->hide();
	}

	return 0;
}

} /* namespace JktPlugin */
