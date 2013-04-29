// Scripting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <stdio.h>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

/* the Lua interpreter */
lua_State* L;

int luaadd ( int x, int y )
{
	int sum;

	/* the function name */
	lua_getglobal(L, "add");

	/* the first argument */
	lua_pushnumber(L, x);

	/* the second argument */
	lua_pushnumber(L, y);

	/* call the function with 2 arguments, return 1 result */
	lua_call(L, 2, 1);

	/* get the result */
	sum = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);

	return sum;
}

static int average(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* loop through each argument */
	for (i = 1; i <= n; i++)
	{
		if (!lua_isnumber(L, i)) {
			lua_pushstring(L, "Incorrect argument to 'average'");
			lua_error(L);
		}

		/* total the arguments */
		sum += lua_tonumber(L, i);
	}

	/* push the average */
	lua_pushnumber(L, sum / n);

	/* push the sum */
	lua_pushnumber(L, sum);

	/* return the number of results */
	return 2;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int sum;

	/* initialize Lua */
	L = lua_open();

	/* load Lua base libraries */
	luaL_openlibs(L);

	/* register our function */
	lua_register(L, "average", average);

	/* load the script */
	if (luaL_dofile(L, "avg.lua") != 0)
	{
		std::cout << "error running function: " << lua_tostring(L, -1) << std::endl;
	}

	/* set 'z' global */
	lua_pushnumber(L, 10);
	lua_setglobal(L, "z");

	/* get 'z' global */
	lua_getglobal(L, "z");
	int z = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);

	/* print 'z' global */
	printf( "Z is %d\n", z );

	/* load the script */
	luaL_dofile(L, "add.lua");

	/* call the add function */
	sum = luaadd( 10, 15 );

	/* print the result */
	printf( "The sum is %d\n", sum );

	/* load the script */
	luaL_dofile(L, "print.lua");

	/* load the script */
	luaL_dofile(L, "table.lua");

	/* cleanup Lua */
	lua_close(L);

	/* pause */
	printf( "Press enter to exit..." );
	getchar();

	return 0;
}

