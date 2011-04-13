require "table_ext"

-- Usage:

-- Create an object/class:
--   object/class = prototype {value, ...; field = value ...}
--   An object's metatable is itself.
--   In the initialiser, unnamed values are assigned to the fields
--   given by _init (assuming the default _clone).
--   Private fields and methods start with "_"

-- Access an object field: object.field
-- Call an object method: object:method (...)
-- Call a class method: Class.method (object, ...)

-- Add a field: object.field = x
-- Add a method: function object:method (...) ... end


-- Root object
Object = {}
local mt = { __tostring = function(self)
		local s = "methods:\n"
		for i,v in pairs(self:methods()) do
			if type(v) == "string" then s = s.."\t"..v.."\n" end
		end
		s = s.."member:\n"
		for i,v in pairs(self:member()) do
			if type(v) == "string" then s = s.."\t"..v..'\n' end
		end
		return s
	end
}
setmetatable(Object, mt)
function Object:methods()
	local ret = {}
	local mt = getmetatable(self)
	if mt and mt.__index then
		ret = mt.__index:methods()
	end
	for i,v in pairs(self) do
		if type(v) == "function" then
			table.insert(ret, i)
		end
	end
	return ret
end
function Object:member()
	local ret = {}
	local mt = getmetatable(self)
	if mt and mt.__index then
		ret = mt.__index:member()
	end
	for i,v in pairs(self) do
		if type(v) ~= "function" then
			table.insert(ret, i)
		end
	end
	return ret
end


function Object:new(...)
	local o = {}
	local mt = {__index = self, __tostring = getmetatable(self) and getmetatable(self).__tostring or nil}
	setmetatable(o, mt)
	if self.__init then
		o:__init(...)
	end
	return o
end
function Object:derive(o)
	local o = o or {}
	local mt = {__index = self, __tostring = getmetatable(self) and getmetatable(self).__tostring or nil}
	setmetatable(o, mt)
	return o
end
--使用userdata会有问题
--function Object:dup()
--	return table.clone(self)
--end
