#pragma once
#include <lauxlib.h>
#include <lua.h>

#if LUA_VERSION_NUM < 504
LUA_API void *(lua_newuserdatauv)(lua_State *L, size_t sz, int nuvalue);
LUA_API int(lua_getiuservalue)(lua_State *L, int idx, int n);
LUA_API int(lua_setiuservalue)(lua_State *L, int idx, int n);
#endif
