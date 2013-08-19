/*
 * PluginNumericProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINNUMERICPROXY_H_
#define PLUGINNUMERICPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunaFive.h"

#include "plugin/api/gui/PluginNumeric.h"

namespace JktPlugin {

class PluginNumericProxy {
	friend class Luna<PluginNumericProxy>;
	static const char className[];
	static Luna<PluginNumericProxy>::FunctionType methods[];

	PluginNumeric* _pluginNumeric;
public:
	PluginNumericProxy(lua_State* L);
	PluginNumericProxy(PluginNumeric* pluginNumeric);
	virtual ~PluginNumericProxy();

	int getValue(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINNUMERICPROXY_H_ */
