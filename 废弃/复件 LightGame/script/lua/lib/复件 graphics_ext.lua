require "graphics"
require "object"
local enc = function(func)
	return function(self, ...) return func(self._ptr, ...) end
end
local spfindcls = function(name)
	return splugin.get_class(name)
end

local spfind_new = function(kls)
	local _ptr = kls._ptr
	return function(...)
		return splugin.new_obj(_ptr, ...)
	end
end

local spfind_delete = function(kls)
	return function(obj)
		return splugin.delete_obj(obj)
	end
end
local spfind = function(kls, name)
	local func = splugin.get_obj_func(kls._ptr, name)
	return function(obj, ...)
		return splugin.call_obj_func(func, obj, ...)
	end
end
cSprite = Object:derive({
	destroy = enc(sprite.remove),
	set_x = enc(sprite.set_x),
	set_y = enc(sprite.set_y),
	get_x = enc(sprite.get_x),
	get_y = enc(sprite.get_y),
	set_ox = enc(sprite.set_ox),
	get_ox = enc(sprite.get_ox),
	set_oy = enc(sprite.set_oy),
	get_oy = enc(sprite.get_oy),
	set_rotation = enc(sprite.set_rotation),
	get_rotation = enc(sprite.get_rotation),
	set_scalex = enc(sprite.set_scalex),
	get_scalex = enc(sprite.get_scalex),
	set_scaley = enc(sprite.set_scaley),
	get_scaley = enc(sprite.get_scaley),
	set_width = enc(sprite.set_width),
	get_width = enc(sprite.get_width),
	set_height = enc(sprite.set_height),
	get_height = enc(sprite.get_height),
	set_color = enc(sprite.set_color),
	get_color = enc(sprite.get_color),
	set_alpha = enc(sprite.set_alpha),
	get_alpha = enc(sprite.get_alpha),
	set_srcrect = enc(sprite.set_srcrect),
	get_srcrect = enc(sprite.get_srcrect),
	flash = enc(sprite.flash),
	flick = enc(sprite.flick)
})
--arg {file=?, z=0, layer=0}
function cSprite:__init(arg)--new = function(self, arg)
	self._ptr = sprite.new(arg.file, arg.z or 0)
end

cFont = Object:derive({
	destroy = enc(text.remove_font)
})
function cFont:__init(arg)
	self._ptr = text.new_font(arg.height, arg.width, arg.weight or 0, arg.facename or 'arial')
end

cText = Object:derive({
	destroy = enc(text.remove),
	set_color = enc(text.set_color),
	get_color = enc(text.get_color),
	set_alpha = enc(text.set_alpha),
	get_alpha = enc(text.get_alpha),
	set_contents = enc(text.set_contents),
	set_rect = enc(text.set_rect)
})
function cText:__init(arg)
	self._ptr = text.new(arg.contents, arg.font._ptr, arg.z or 0)
end
