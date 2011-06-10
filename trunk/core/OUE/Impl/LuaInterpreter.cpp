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
#include "LuaInterpreter.h"
#include "../../Base/String.h"
namespace OneU
{
	LuaInterpreter::LuaInterpreter(){
		L = lua_open();
		luaL_openlibs(L);
		lua_getglobal(L, "package");
		lua_pushstring(L, ";./script/?.lua;./script/lib/?.lua");
		lua_setfield(L, -2, "path");
		lua_pushstring(L, ";./../release/?.dll");
		lua_setfield(L, -2, "cpath");
		lua_pop(L, 1);
	}
	void LuaInterpreter::execFile(pcwstr filename){
		if(luaL_loadfile(L, Wide2ANSI(filename)))
			ONEU_RAISE(ANSI2Wide(lua_tostring(L, -1)));
		if(lua_pcall(L, 0, 0, 0))
			ONEU_RAISE(ANSI2Wide(lua_tostring(L, -1)));
	}
	void LuaInterpreter::execCode(pcstr code){
		if(luaL_loadstring(L, code))
			ONEU_RAISE(ANSI2Wide(lua_tostring(L, -1)));
		if(lua_pcall(L, 0, 0, 0))
			ONEU_RAISE(ANSI2Wide(lua_tostring(L, -1)));
	}

	ONEU_API IInterpreter* Interpreter_Lua(){
		return ONEU_NEW LuaInterpreter;
	}
}