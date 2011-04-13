require "graphics"
require "object"
require "spext"

renderer = { width = 800, height = 600}

local oldenv = splugin.bind_env(splugin.get("graphics"))

--cSprite
cSprite = spCls("sprite", {"set_x", "set_y", "get_x", "get_y", "set_ox", "set_oy", "get_ox", "get_oy",
	"set_rotation", "get_rotation", "set_scalex", "get_scalex", "set_scaley", "get_scaley",
	"set_width", "get_width", "set_height", "get_height", "set_color", "get_color", "set_alpha", "get_alpha",
	"set_srcrect", "get_srcrect", "flash", "flick"})
--arg {file=?, z=0, layer=0}
cSprite.__new = cSprite.__init
function cSprite:__init(arg)--new = function(self, arg)
	self:__new(arg.file, arg.z or 0)
end
cEffect = spCls("effect", {})
cEffect.__new = cEffect.__init
function cEffect:__init(sprite)
	self:__new(sprite._ptr)
end
--cFont
cFont = spCls("font", {})
cFont.__new = cFont.__init
function cFont:__init(arg)
	self:__new(arg.height, arg.width, arg.weight or 0, arg.facename or 'arial')
end


cText = spCls("text", {"set_color", "get_color", "set_alpha", "get_alpha", "set_contents", "set_rect"})
cText.set_rect_i = cText.set_rect
cText.set_rect = function(self, arg)
	self:set_rect_i(arg.l, arg.t, arg.r, arg.b)
end
cText.__new = cText.__init
function cText:__init(arg)
	self:__new(arg.contents, arg.z or 0, arg.font._ptr)
end

splugin.bind_env(oldenv)
