#include "lua_userdata_compat.h"
#include <assert.h>

#if LUA_VERSION_NUM < 504
#define NUVALUE_KEY 0

LUA_API void *lua_newuserdatauv(lua_State *L, size_t size, int nuvalue) {
  void *u = lua_newuserdata(L, size);
  if (nuvalue == 0) {
    return u;
  }

  lua_createtable(L, nuvalue, 1);
  lua_pushinteger(L, nuvalue);
  lua_rawseti(L, -2, NUVALUE_KEY);

  lua_setuservalue(L, -2);
  assert(lua_type(L, -1) == LUA_TUSERDATA);
  return u;
}

static int getnuvalue(lua_State *L, int idx) {
  lua_rawgeti(L, idx, NUVALUE_KEY);
  if (!lua_isnumber(L, -1)) {
    luaL_error(L, "Missing lua userdata compat nuvalue");
  }

  int nuvalue = (int)lua_tointeger(L, -1);
  lua_pop(L, 1);
  return nuvalue;
}

LUA_API int lua_getiuservalue(lua_State *L, int idx, int n) {
  assert(lua_type(L, idx) == LUA_TUSERDATA);
  if (lua_getuservalue(L, idx) != LUA_TTABLE) {
    luaL_error(L, "Missing lua userdata compat table");
  }

  int nuvalue = getnuvalue(L, -1);
  if (n <= 0 || n > nuvalue) {
    lua_pop(L, 1);
    lua_pushnil(L);
    return LUA_TNONE;
  }

  int t = lua_rawgeti(L, -1, n);
  assert(lua_type(L, -2) == LUA_TTABLE);
  lua_remove(L, -2);
  return t;
}

LUA_API int lua_setiuservalue(lua_State *L, int idx, int n) {
  assert(lua_type(L, idx) == LUA_TUSERDATA);
  if (lua_getuservalue(L, idx) != LUA_TTABLE) {
    luaL_error(L, "Missing lua userdata compat table");
  }

  int nuvalue = getnuvalue(L, -1);
  if (n <= 0 || n > nuvalue) {
    lua_pop(L, 1);
    return 0;
  }

  lua_insert(L, -2);
  lua_rawseti(L, -2, n);

  assert(lua_type(L, -1) == LUA_TTABLE);
  lua_pop(L, 1);
  return 1;
}
#endif