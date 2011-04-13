require "object"

--Atom.getEnv() - return env
--Atom.getSymbol(env or klass, string) - return value
--Atom.call(function, ...) - return retvals
--Atom.createObj(klass, ...) -return obj
--Atom.callObjFunc(obj, obj_function, ...) -return retvals
--Atom.type(value) -- return Atom.T_FUNC ... etc.
--Atom.traversalEnv(env or klass, function(symbol, value))
--Atom.getKlass(obj) -return klass
--Atom.loadLib(libpath) - load a Atom.dll
--Atom.log(string) - log to log file


--lua内置变量为拷贝
--其余为引用
--Atom库的原则是尽量使用拷贝的方式从表中导入数据。
--只有无法被lua内置支持的才会是引用，一般而言是常量引用。
--简而言之就是在大多数情况下表内数据对于lua是个常量。
--lua修改表内数据的唯一方式是删掉符号或创建新的符号。
Atom = {}

Atom.makeKlass = function(kv)
	if(Atom.type(kv) ~= Atom.T_KLS) then
		error("The Atom value is not a klass!")
	end
	local k = {
		__init = function(self, ...) self._ptr = Atom.createObj(kv, ...) end
	}
	Atom.traversalEnv(kv,
		function(name, value)
			if(type(value) == "userdata") then
				if(Atom.type(value) == Atom.T_OBJFUNC) then
					k[name] = function(self, ...)
						return Atom.callObjFunc(self._ptr, value, ...)
					end
				end
				--else if(Atom.type(value)--未完成
			else k[name] = value end
		end)
	return Object:derive(k)
end

Atom.makeObj = function(ov)
	if(Atom.type(ov) ~= Atom.T_OBJ) then
		error("The Atom value is not a object!")
	end
	local kv = Atom.getKlass(ov)
	local o = Atom.makeKlass(kv);
	--不记录当前makeKlass导入的value存在一定风险
	--如果Klass表中含有Obj，而该Obj碰巧又是该Klass的Obj，就会不停递归。
	--@todo 记录每一个导入的Environment，避免重复导入。
	o._ptr = ov;
	return o;
end

Atom.makeFunction = function(fv)
	return function(...)
		return Atom.call(fv, ...)
	end
end


Atom.S = function(symbol)
	return Atom.getSymbol(Atom.getEnv(), symbol)
end

function l(obj)
	return obj._ptr
end
