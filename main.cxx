// https://gist.github.com/kingluo/ad5c40047bd41ab05d5e

// test.lua
//
// local total = 0
// for i=0,10000-1 do
//   total = total + i
// end
// return total

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <assert.h>

int main(int argc, char *argv[])
{
    const int max = 10000;

    int i = 0;
    if (argc > 1)
    {
        puts("Running single thread");

        // Single threaded
        for (i = 0; i < max; i++)
        {
            lua_State *L = luaL_newstate();
            luaL_openlibs(L);

            if (luaL_loadfile(L, "test.lua") != 0)
            {
                puts("loadfile(test.lua) failed");
                return 1;
            }

            int nres;
            lua_resume(L, NULL, 0, &nres);
            assert(lua_tointeger(L, 1) == 49995000);

            lua_close(L);
        }
    }
    else
    {
        puts("Running multi thread");

        lua_State *L = luaL_newstate();
        luaL_openlibs(L);
        luaL_newmetatable(L, "gtm");
        lua_pushglobaltable(L);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);

        for (i = 0; i < max; i++)
        {
            // Creates a new thread, pushes it on the stack, and returns a pointer to a lua_State that represents this new thread.
            // The new thread returned by this function shares with the original thread its global environment,
            // but has an independent execution stack.
            lua_State *co = lua_newthread(L);

            if (luaL_loadfile(co, "test.lua") != 0)
            {
                puts("loadfile(test.lua) failed");
                return 1;
            }

            lua_newtable(co);
            luaL_setmetatable(co, "gtm");
            lua_setupvalue(co, 1, 1);

            int nres;
            lua_resume(co, NULL, 0, &nres);
            assert(lua_tointeger(co, 1) == 49995000);
            lua_settop(L, 0);
            lua_gc(L, LUA_GCCOLLECT, 0);
        }

        lua_close(L);
    }
    return 0;
}
