/*
    This file is part of Iceball.

    Iceball is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Iceball is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Iceball.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

struct icelua_entry {
	int (*fn) (lua_State *L);
	char *name;
};

lua_State *lstate_client = NULL;
lua_State *lstate_server = NULL;

// helper functions
int icelua_assert_stack(lua_State *L, int smin, int smax)
{
	int top = lua_gettop(L);
	
	if(smin != -1 && top < smin)
		return luaL_error(L, "expected at least %d arguments, got %d\n", smin, top);
	if(smax != -1 && top > smax)
		return luaL_error(L, "expected at most %d arguments, got %d\n", smax, top);
	
	return top;
}

// common functions
int icelua_fn_common_get_map_dims(lua_State *L)
{
	int top = icelua_assert_stack(L, 0, 0);
	
	map_t *map = (L == lstate_server ? svmap : clmap);
	
	// if no map, just give off nils
	if(map == NULL)
	{
		return 0;
	} else {
		lua_pushinteger(L, map->xlen);
		lua_pushinteger(L, map->ylen);
		lua_pushinteger(L, map->zlen);
		return 3;
	}
}

int icelua_fn_common_get_map_pillar(lua_State *L)
{
	int top = icelua_assert_stack(L, 2, 2);
	int px, pz;
	int i;
	
	px = lua_tointeger(L, 1);
	pz = lua_tointeger(L, 2);
	
	map_t *map = (L == lstate_server ? svmap : clmap);
	
	// if no map, return nil
	if(map == NULL)
		return 0;
	
	// get a pillar
	uint8_t *p = map->pillars[(pz&(map->zlen-1))*map->xlen+(px&(map->xlen-1))];
	
	// build the list
	int llen = 4*(1+(int)*p);
	lua_createtable(L, llen, 0);
	p += 4;
	
	for(i = 1; i <= llen; i++)
	{
		lua_pushinteger(L, i);
		lua_pushinteger(L, *(p++));
		lua_settable(L, -3);
	}
	
	return 1;
}
// client functions
int icelua_fn_client_camera_point(lua_State *L)
{
	int top = icelua_assert_stack(L, 3, 5);
	float dx, dy, dz;
	float zoom = 1.0f, roll = 0.0f;
	
	dx = lua_tonumber(L, 1);
	dy = lua_tonumber(L, 2);
	dz = lua_tonumber(L, 3);
	if(top <= 4)
		zoom = lua_tonumber(L, 4);
	if(top <= 5)
		roll = lua_tonumber(L, 5);
	
	cam_point_dir(&tcam, dx, dy, dz, zoom, roll);
	
	return 0;
}

int icelua_fn_client_camera_move_local(lua_State *L)
{
	int top = icelua_assert_stack(L, 3, 3);
	float dx, dy, dz;
	
	dx = lua_tonumber(L, 1);
	dy = lua_tonumber(L, 2);
	dz = lua_tonumber(L, 3);
	
	tcam.mpx += dx*tcam.mxx+dy*tcam.myx+dz*tcam.mzx;
	tcam.mpy += dx*tcam.mxy+dy*tcam.myy+dz*tcam.mzy;
	tcam.mpz += dx*tcam.mxz+dy*tcam.myz+dz*tcam.mzz;

	return 0;
}

int icelua_fn_client_camera_move_global(lua_State *L)
{
	int top = icelua_assert_stack(L, 3, 3);
	float dx, dy, dz;
	
	dx = lua_tonumber(L, 1);
	dy = lua_tonumber(L, 2);
	dz = lua_tonumber(L, 3);
	
	tcam.mpx += dx;
	tcam.mpy += dy;
	tcam.mpz += dz;

	return 0;
}

int icelua_fn_client_camera_move_to(lua_State *L)
{
	int top = icelua_assert_stack(L, 3, 3);
	float px, py, pz;
	
	px = lua_tonumber(L, 1);
	py = lua_tonumber(L, 2);
	pz = lua_tonumber(L, 3);
	
	tcam.mpx = px;
	tcam.mpy = py;
	tcam.mpz = pz;

	return 0;
}

int icelua_fn_client_camera_get_pos(lua_State *L)
{
	int top = icelua_assert_stack(L, 0, 0);
	
	lua_pushinteger(L, tcam.mpx);
	lua_pushinteger(L, tcam.mpy);
	lua_pushinteger(L, tcam.mpz);
	
	return 3;
}

int icelua_fn_client_camera_get_forward(lua_State *L)
{
	int top = icelua_assert_stack(L, 0, 0);
	
	lua_pushinteger(L, tcam.mzx);
	lua_pushinteger(L, tcam.mzy);
	lua_pushinteger(L, tcam.mzz);
	
	return 3;
}

// server functions

struct icelua_entry icelua_client[] = {
	{icelua_fn_client_camera_point, "camera_point"},
	{icelua_fn_client_camera_move_local, "camera_move_local"},
	{icelua_fn_client_camera_move_global, "camera_move_global"},
	{icelua_fn_client_camera_move_to, "camera_move_to"},
	{icelua_fn_client_camera_get_pos, "camera_get_pos"},
	{icelua_fn_client_camera_get_forward, "camera_get_forward"},
	{NULL, NULL}
};

struct icelua_entry icelua_server[] = {
	{NULL, NULL}
};
struct icelua_entry icelua_common[] = {
	{icelua_fn_common_get_map_dims, "get_map_dims"},
	{icelua_fn_common_get_map_pillar, "get_map_pillar"},
	{NULL, NULL}
};

struct icelua_entry icelua_common_client[] = {
	{NULL, NULL}
};

struct icelua_entry icelua_common_server[] = {
	{NULL, NULL}
};

void icelua_loadfuncs(lua_State *L, char *table, struct icelua_entry *fnlist)
{
	lua_getglobal(L, table);
	
	while(fnlist->fn != NULL)
	{
		lua_pushcfunction(L, fnlist->fn);
		lua_setfield (L, -2, fnlist->name);
		fnlist++;
	}
	
	lua_pop(L, 1);
}

void icelua_loadbasefuncs(lua_State *L)
{
	// load base library
	// TODO: whitelist the functions by spawning a new environment.
	// this is harder than it sounds.
	lua_pushcfunction(L, luaopen_base);
	lua_call(L, 0, 0);
	
	// here's the other two
	lua_pushcfunction(L, luaopen_string);
	lua_call(L, 0, 0);
	lua_pushcfunction(L, luaopen_math);
	lua_call(L, 0, 0);
}

int icelua_init(void)
{
	// create states
	lstate_client = luaL_newstate();
	lstate_server = luaL_newstate();
	
	// create tables
	lua_newtable(lstate_client);
	lua_setglobal(lstate_client, "client");
	lua_newtable(lstate_client);
	lua_setglobal(lstate_client, "common");
	
	lua_newtable(lstate_server);
	lua_setglobal(lstate_server, "server");
	lua_newtable(lstate_server);
	lua_setglobal(lstate_server, "common");
	
	// load stuff into them
	icelua_loadfuncs(lstate_client, "client", icelua_client);
	icelua_loadfuncs(lstate_server, "server", icelua_server);
	icelua_loadfuncs(lstate_client, "client", icelua_common);
	icelua_loadfuncs(lstate_server, "server", icelua_common);
	icelua_loadfuncs(lstate_client, "common", icelua_common);
	icelua_loadfuncs(lstate_server, "common", icelua_common);
	icelua_loadfuncs(lstate_client, "client", icelua_common_client);
	icelua_loadfuncs(lstate_server, "server", icelua_common_server);
	icelua_loadfuncs(lstate_client, "common", icelua_common_client);
	icelua_loadfuncs(lstate_server, "common", icelua_common_server);
	
	// load some lua base libraries
	icelua_loadbasefuncs(lstate_client);
	icelua_loadbasefuncs(lstate_server);
	
	/*
	NOTE:
	to call stuff, use lua_pcall.
	DO NOT use lua_call! if it fails, it will TERMINATE the program!
	*/
	
	// quick test
	// TODO: set up a "convert/filter file path" function
	// TODO: split the client/server inits
	if(luaL_loadfile(lstate_server, "pkg/base/main_server.lua") != 0)
	{
		printf("ERROR loading server Lua: %s\n", lua_tostring(lstate_server, -1));
		return 1;
	}
	
	if(luaL_loadfile(lstate_client, "pkg/base/main_client.lua") != 0)
	{
		printf("ERROR loading client Lua: %s\n", lua_tostring(lstate_client, -1));
		return 1;
	}
	
	if(lua_pcall(lstate_server, 0, 0, 0) != 0)
	{
		printf("ERROR running server Lua: %s\n", lua_tostring(lstate_server, -1));
		lua_pop(lstate_server, 1);
	}
	lua_pop(lstate_server, 1);
	
	if(lua_pcall(lstate_client, 0, 0, 0) != 0)
	{
		printf("ERROR running client Lua: %s\n", lua_tostring(lstate_client, -1));
		lua_pop(lstate_client, 1);
	}
	lua_pop(lstate_client, 1);
	
	return 0;
}

void icelua_deinit(void)
{
	// TODO!
}
