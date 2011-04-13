-- temportary libs
-- low efficiency
require "graphics_ext"

cWindow = Object:derive({})
cWindow.skinfile = "windows.png" -- set the skin
cWindow.font = cFont:new{facename = "ariel", width = 10, height = 20} -- set the font

function cWindow:__init(rc, z)
	local skinfile = cWindow.skinfile
	self.g = renderer.new_group(z)
	renderer.open_group(self.g)
	self.back = cSprite:new{file = skinfile, z = 1 }

	self.border_top = cSprite:new{file = skinfile, z = 2}
	self.border_bottom = cSprite:new{file = skinfile, z = 2}
	--self.border_left = cSprite:new{file = skinfile, z = 2}
	--self.border_right = cSprite:new{file = skinfile, z = 2}
	--self.border_lt = cSprite:new{file = skinfile, z = 2}
	--self.border_lb = cSprite:new{file = skinfile, z = 2}
	--self.border_rt = cSprite:new{file = skinfile, z = 2}
	--self.border_rb = cSprite:new{file = skinfile, z = 2}
	renderer.close_group()

	local width = rc.r - rc.l
	local height = rc.b - rc.t

	self.back:set_srcrect(0.0, 0.0, 5.0 / 16, 5.0 / 8)

	self.back:set_x(rc.l)
	self.back:set_y(rc.t)
	self.back:set_width(width)
	self.back:set_height(height)

	self.back:set_alpha(128)

	self.border_top:set_srcrect(6.0 / 16, 0, 7.0 / 16, 1.0 / 8)
	self.border_top:set_x(rc.l)
	self.border_top:set_y(rc.t)
	self.border_top:set_width(width)


	self.border_bottom:set_srcrect(6.0 / 16, 3.0 / 8, 7.0 / 16, 4.0 / 8)

	self.border_bottom:set_x(rc.l)
	self.border_bottom:set_y(rc.b)
	self.border_bottom:set_width(width)
	self.border_bottom:set_oy(self.border_bottom:get_height())

	self.text = cText:new{contents = "", z = 3, font = cWindow.font}
	self.text:set_rect{l = rc.l + 3, t = rc.t + 3, r = rc. r - 3, b = rc.b - 3}
end

function cWindow:destroy()
	self.border_top:destroy()
	self.border_bottom:destroy()
	self.back:destroy()
	renderer.remove_group(self.g)
end

function cWindow:set_back_alpha(alpha)
	self.back:set_alpha(alpha)
end

function cWindow:set_contents(contents)
	self.text.set_contents(contents)
end
