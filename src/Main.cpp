#include "Game.h"

#include <iostream>


int main()
{
    lua_State* L = luaL_newstate();
    luaL_dofile(L, "assets/scripts/script.lua");
    luaL_openlibs(L);
    lua_pcall(L, 0, 0, 0);

    Game game;
    game.run();
    return 0;
}