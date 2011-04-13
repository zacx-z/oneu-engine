require "Object"
spFunc = function(name)
	local func = splugin.get(name)
	return function(...)
		return splugin.call_func(func, ...)
	end
end
spFuncP = function(pack, list)
	for i, v in ipairs(list) do
		pack[v] = spFunc(v)
	end
end
spCls = function(name, funlist)
	local kls = splugin.get(name)
	local o = {__init = function(self, ...) self._ptr = splugin.new_obj(kls, ...) end,
		destroy = function(self) splugin.delete_obj(self._ptr) end
		}

	local get_obj_func = function(name)
		local func = splugin.get_obj_func(kls, name)
		return function(self, ...) return splugin.call_obj_func(func, self._ptr, ...) end
	end
	for i, v in ipairs(funlist) do
		o[v] = get_obj_func(v)
	end
	return Object:derive(o)
end
