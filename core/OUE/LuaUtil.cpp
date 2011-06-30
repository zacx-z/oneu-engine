/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "LuaUtil.h"
namespace OneU
{
	lua_State* LuaOpen(){
		lua_State* L = lua_open();
		luaL_openlibs(L);
		luaL_dostring(L, "package.path = package.path..\";./script/?.lua;./script/lib/?.lua\"");

#ifdef _DEBUG
		luaL_dostring(L, "package.cpath = package.cpath..\";./?.dll;./../debug/?.dll\"");//for test
#else
		luaL_dostring(L, "package.cpath = package.cpath..\";./?.dll\"");//for test
#endif
		lua_setfield(L, -2, "cpath");
		lua_pop(L, 1);
		return L;
	}
	void LuaExecFile(lua_State* L, pcwstr filename){
		if(luaL_loadfile(L, Wide2Char(filename)))
			ONEU_RAISE(Char2Wide(lua_tostring(L, -1)));
		if(lua_pcall(L, 0, 0, 0))
			ONEU_RAISE(Char2Wide(lua_tostring(L, -1)));
	}
	void LuaExecCode(lua_State* L, pcstr code){
		if(luaL_loadstring(L, code))
			ONEU_RAISE(Char2Wide(lua_tostring(L, -1)));
		if(lua_pcall(L, 0, 0, 0))
			ONEU_RAISE(Char2Wide(lua_tostring(L, -1)));
	}
}