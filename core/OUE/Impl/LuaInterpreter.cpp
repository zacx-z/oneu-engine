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
#include "Atom_Impl.h"

#ifdef _DEBUG
#pragma comment(lib, "luaLib_Debug.lib")
#else
#pragma comment(lib, "luaLib.lib")
#endif
namespace OneU
{
 	
// --Atom.getEnv() - return env
// --Atom.getSymbol(env or klass or obj, string) - return value
// --Atom.call(function, ...) - return retvals OR Atom.call(obj_function, obj, ...) - return retvals OR Atom.call(klass, ...) - return obj 创建对象(使用createObj实现)
// --Atom.createObj(klass, ...) -return obj
// --Atom.type(value) -- return Atom.T_FUNC ... etc.
// --Atom.traversalEnv(env or klass, function(symbol, value))
//// --Atom.getKlass(obj) -return klass 已取消
// --Atom.loadLib(libpath) - load a Atom.dll
// --Atom.log(string) - log to log file
// --Atom.destroy(atom_v) - destroy value immediately
	namespace atom
	{
		int ErrorFunc(lua_State* L);

		static const luaL_Reg DefaultLib[] = {
			{"oneu_errfunc", ErrorFunc},
			{NULL, NULL}
		};

		//某些函数在metatable里也有使用，慎重修改原型
		int atGetEnv(lua_State* L);
		int atGetSymbol(lua_State* L);
		int atCall(lua_State* L);
		int atMakeObj(lua_State* L);
		int atType(lua_State* L);
		int atTraversal(lua_State* L);
		//int atGetKlass(lua_State* L);
		int atLoadLib(lua_State* L);
		int atLog(lua_State* L);
		int atRemoveValue(lua_State* L);

		int _atCallObjFunc(lua_State* L);
		//int _atGetProperty(lua_State* L);
		//int _atPropertySet(lua_State* L);
		//int _atPropertyGet(lua_State* L);

		static const luaL_Reg AtomLib[] = {
			{"getEnv", atGetEnv},
			{"getSymbol", atGetSymbol},
			{"call", atCall},
			{"createObj", atMakeObj},
			{"type", atType},
			{"log", atLog},
			{"traversalEnv", atTraversal},
		//	{"getKlass", atGetKlass},//from obj
			{"loadLib", atLoadLib},
			{"destroy", atRemoveValue},
			{NULL, NULL}
		};

		LuaInterpreter::LuaInterpreter()
		{
			m_L = lua_open();
			luaL_openlibs(m_L);

			//设定路径
			lua_getglobal(m_L, "package");
			lua_pushstring(m_L, ";./script/lua/?.lua;./script/lua/lib/?.lua");
			lua_setfield(m_L, -2, "path");

			//注册默认库
			luaL_register(m_L, "_G", DefaultLib);

			//注册Atom类型
			luaL_register(m_L, "Atom", AtomLib);
			//注册Atom常量
			lua_pushinteger(m_L, T_FUNC);
			lua_setfield(m_L, -2, "T_FUNC");
			//
			lua_pushinteger(m_L, T_OBJFUNC);
			lua_setfield(m_L, -2, "T_OBJFUNC");
			//
			lua_pushinteger(m_L, T_ENV);
			lua_setfield(m_L, -2, "T_ENV");
			//
			lua_pushinteger(m_L, T_KLS);
			lua_setfield(m_L, -2, "T_KLS");
			//
			lua_pushinteger(m_L, T_OBJ);
			lua_setfield(m_L, -2, "T_OBJ");
			//
			lua_pushinteger(m_L, T_CPOINTER);
			lua_setfield(m_L, -2, "T_CPOINTER");
			//注册atom_value
			luaL_newmetatable(m_L, "atom_value");

			lua_pushcfunction(m_L, atRemoveValue);
			lua_setfield(m_L, -2, "__gc");

			lua_pushcfunction(m_L, atCall);
			lua_setfield(m_L, -2, "__call");

			lua_pushcfunction(m_L, atGetSymbol);
			lua_setfield(m_L, -2, "__index");

			//lua_pushcfunction(m_L, _atGetProperty);
			//lua_setfield(m_L, -2, "__concat");
			
			lua_pop(m_L, 1);

			//注册atom_property
			//luaL_newmetatable(m_L, "atom_property");

			//lua_pushcfunction(m_L, _atPropertySet);
			//lua_setfield(m_L, -2, "__call");

			//lua_pushcfunction(m_L, _atPropertyGet);
			//lua_setfield(m_L, -2, "__len");

			//lua_pop(m_L, 1);
			//代码部分
			//简化getEnv
			luaL_dostring(m_L, "Atom.G = Atom.getEnv()");
		}
		LuaInterpreter::~LuaInterpreter()
		{
			if(m_L != NULL)
				lua_close(m_L);
			GetAtom().gc();
		}

		void LuaInterpreter::execFile(pcwstr filename)
		{
			beforeExec();
			lua_getglobal(m_L, "oneu_errfunc");

			if(luaL_loadfile(m_L, Wide2ANSI(filename)))
				ONEU_RAISE(ANSI2Wide(lua_tostring(m_L, -1)));
			if(lua_pcall(m_L, 0, 0, -2))
				ONEU_RAISE(ANSI2Wide(lua_tostring(m_L, -1)));

			afterExec();
		}

		value* LuaInterpreter::exec(pcwstr code)
		{
			beforeExec();

			lua_getglobal(m_L, "oneu_errfunc");
			if(luaL_loadstring(m_L, Wide2ANSI(code)))
				ONEU_RAISE(ANSI2Wide(lua_tostring(m_L, -1)));
			if(lua_pcall(m_L, 0, 0, -2))
				ONEU_RAISE(ANSI2Wide(lua_tostring(m_L, -1)));

			afterExec();
			return NULL;
		}

		void LuaInterpreter::_getStackInfo(String& buffer)
		{
			lua_getglobal(m_L, "debug");
			lua_getfield(m_L, -1, "traceback");
			lua_call(m_L, 0, 1);
			buffer.append(ANSI2Wide(lua_tostring(m_L, -1)));
			lua_pop(m_L, 3);
		}


		//错误函数
		int ErrorFunc(lua_State* L){
			lua_getglobal(L, "debug");
			lua_getfield(L, -1, "traceback");
			lua_remove(L, -2);
			lua_insert(L, 1);
			lua_call(L, 1, 1);
			return 1;
		}
		//Atom库
		value* _atGetValue(lua_State* L){
			switch(lua_type(L, -1))
			{
			case LUA_TNIL:
				return makeNil();
			case LUA_TBOOLEAN:
				return makeValue((bool)lua_toboolean(L, -1));
			case LUA_TNUMBER:
				return makeValue((float)lua_tonumber(L, -1));
			case LUA_TSTRING:
				return makeValue((pcwstr)ANSI2Wide(lua_tostring(L, -1)));
				//未做
			case LUA_TTABLE:
			case LUA_TFUNCTION:
				return NULL;
			}
			return NULL;
		}
		void _atPushValue(lua_State* L, value* v){
			switch(v->type())
			{
			case T_NIL:
				lua_pushnil(L);
				break;
			case T_BOOL:
				lua_pushboolean(L, *v->get<bool>());
				break;
			case T_INT:
				lua_pushinteger(L, *v->get<int>());
				break;
			case T_FLOAT:
				lua_pushnumber(L, *v->get<float>());
				break;
			case T_STR:
				lua_pushstring(L, Wide2ANSI(v->get<String>()->c_str()));
				break;
			case T_TUPLE:
				//未处理
			default:
				if(!v->acquire()) ATOM_RAISE(L"Error!");
				value** ppv = (value**)lua_newuserdata(L, sizeof(value*));
				*ppv = v;

				luaL_getmetatable(L, "atom_value");
				lua_setmetatable(L, -2);
			}
			
		}
		//在b到e的左闭右开区间内转化参数
		value* _atGetArgs(lua_State* L, int b, int e){
			value* ret = makeTuple(e - b);
			tuple* t = ret->get<tuple>();
			for(int i = b; i < e; ++i){
				switch(lua_type(L, i))
				{
				case LUA_TNIL:
					t->get(i - b)->evalNil();
					break;
				case LUA_TBOOLEAN:
					t->get(i - b)->eval((bool)lua_toboolean(L, i));
					break;
				case LUA_TNUMBER:
					t->get(i - b)->eval((float)lua_tonumber(L, i));
					break;
				case LUA_TSTRING:
					t->get(i - b)->eval((pcwstr)ANSI2Wide(lua_tostring(L, i)));
					break;
				case LUA_TUSERDATA:
					luaL_checkudata(L, i, "atom_value");
					t->get(i - b)->eval(*(value**)lua_touserdata(L, i));
					break;
				default:
					;
				}
			}
			return ret;
		}
		int _atPushRet(lua_State* L, value* v){
			if(v == NULL)
				return 0;
			if(v->type() != T_TUPLE){
				_atPushValue(L, v);
				return 1;
			}
			else{
				tuple * t= v->get<tuple>();
				for(uint32 i = 0; i < t->length(); ++i){
					_atPushValue(L, t->get(i));
				}
				return t->length();
			}
		}
		int atGetEnv(lua_State* L){
			_atPushValue(L, makeRef(GetAtom().getEnvValue()));

			return 1;
		}
		//value GetSymbol(value<env>)
		int atGetSymbol(lua_State* L){
			luaL_checkudata(L, 1, "atom_value");
			luaL_checkstring(L, 2);

			value* p = *(value**)lua_touserdata(L, 1);

			value* ret;
			if(p->type() == T_KLS)
				ret = p->get<klass>()->getSymbol(ANSI2Wide(lua_tostring(L, 2)));
			else if(p->type() == T_OBJ)
				ret = p->get<obj>()->getKlass()->getSymbol(ANSI2Wide(lua_tostring(L, 2)));
			else
				ret = uget<env>(p).getSymbol(ANSI2Wide(lua_tostring(L, 2)));
			if(ret == NULL) return 0;
			_atPushValue(L, makeRef(ret));
			return 1;
		}
		int atCall(lua_State* L){
			luaL_checkudata(L, 1, "atom_value");
			value* f = *(value**)lua_touserdata(L, 1);
			if(f->type() == T_FUNC)
			{
				value* arg = _atGetArgs(L, 2, lua_gettop(L) + 1);
				value* ret = uget<function>(f).call(arg);
				return _atPushRet(L, ret);
			}
			else if(f->type() == T_OBJFUNC)
			{
				return _atCallObjFunc(L);
			}
			else
				return atMakeObj(L);
		}
		int atMakeObj(lua_State* L){
			luaL_checkudata(L, 1, "atom_value");
			value* arg = _atGetArgs(L, 2, lua_gettop(L) + 1);
			_atPushValue(L, makeObj(&uget<klass>(*(value**)lua_touserdata(L, 1)), arg));
			return 1;
		}
		int atType(lua_State* L){
			luaL_checkudata(L, 1, "atom_value");
			lua_pushinteger(L, (*(value**)lua_touserdata(L, 1))->type());
			return 1;
		}
		struct luaTraversal
		{
			lua_State* L;
			luaTraversal(lua_State* L)
				: L(L){}
			void operator()(pcwstr key, value* v){
				lua_getglobal(L, "oneu_errfunc");
				lua_getfield(L, 2, "__1");
				lua_pushstring(L, Wide2ANSI(key));
				_atPushValue(L, makeRef(v));
				if(lua_pcall(L, 2, 0, -4))
					ONEU_RAISE(ANSI2Wide(lua_tostring(L, -1)));
			}
		};
		//env, function(key, atom_value)
		int atTraversal(lua_State* L){
			luaL_checkudata(L, 1, "atom_value");
			luaL_checktype(L, 2, LUA_TFUNCTION);

			lua_settop(L, 2);
			lua_getglobal(L, "Atom");
			lua_insert(L, 2);
			lua_setfield(L, 2, "__1");//使用Atom表的变量来搞了……
			value* v = *(value**)lua_touserdata(L, 1);
			if(v->type() == T_KLS)
				v->get<klass>()->traversal(luaTraversal(L));
			else if(v->type() == T_OBJ)
				v->get<obj>()->getKlass()->traversal(luaTraversal(L));
			else
				uget<env>(v).traversal(luaTraversal(L));

			lua_settop(L, 1);
			return 0;
		}
		//int atGetKlass(lua_State* L){
		//	luaL_checkudata(L, 1, "atom_value");

		//	_atPushValue(L, makeValueFromPtr(uget<obj>(*(value**)lua_touserdata(L, 1)).getKlass()));
		//	return 1;
		//}
		int atLoadLib(lua_State* L){
			luaL_checkstring(L, 1);
			GetAtom().loadAtomLib(ANSI2Wide(lua_tostring(L, 1)));
			return 0;
		}
		int atLog(lua_State* L){
			luaL_checkstring(L, 1);
			GetLogger().logMessage(lua_tostring(L, 1));
			GetLogger().logMessage(L"\n");
			return 0;
		}
		int atRemoveValue(lua_State* L){
			(*(value**)lua_touserdata(L, 1))->unacquire();
			lua_pushnil(L);
			lua_setmetatable(L, 1);
			return 0;
		}


		int _atCallObjFunc(lua_State* L){
			luaL_checkudata(L, 1, "atom_value");
			luaL_checkudata(L, 2, "atom_value");
			value* arg = _atGetArgs(L, 3, lua_gettop(L) + 1);

			value* ret = uget<obj_function>(*(value**)lua_touserdata(L, 1)).call(&uget<obj>(*(value**)lua_touserdata(L, 2)), arg);
			return _atPushRet(L, ret);
		}
		//int _atGetProperty(lua_State* L){
		//	luaL_checkudata(L, 1, "atom_value");
		//	luaL_checkstring(L, 2);
		//	lua_settop(L, 2);

		//	lua_createtable(L, 2, 0);
		//	lua_insert(L, 1);
		//	lua_rawseti(L, 1, 2);
		//	lua_rawseti(L, 1, 1);

		//	luaL_getmetatable(L, "atom_property");
		//	lua_setmetatable(L, 1);
		//	return 1;
		//}
		//int _atPropertyGet(lua_State* L){
		//	lua_rawgeti(L, 1, 1);
		//	lua_pushstring(L, "get");
		//	lua_rawgeti(L, 1, 2);
		//	lua_concat(L, 2);
		//	value* ret = ucall(&uget<obj>(*(value**)lua_touserdata(L, -2)), ANSI2Wide(lua_tostring(L, -1)), NULL);
		//	return _atPushRet(L, ret);
		//}
		//int _atPropertySet(lua_State* L){
		//	lua_rawgeti(L, 1, 1);
		//	lua_pushstring(L, "set");
		//	lua_rawgeti(L, 1, 2);
		//	lua_concat(L, 2);
		//	value* ret = ucall(&uget<obj>(*(value**)lua_touserdata(L, -2)), ANSI2Wide(lua_tostring(L, -1)), _atGetArgs(L, 2, 3));
		//	ATOM_RAISE(L"haha");
		//	return _atPushRet(L, ret);
		//}

	}
}