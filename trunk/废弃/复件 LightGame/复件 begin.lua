require "graphics_ext"
require "input"
haha = 1
p = cSprite:new{file = "../Author.png"}
aa = cSprite:new{file = "../Author.png"}
p:set_ox(100)
p:set_oy(100)
scalex = 1
frame = 0
commander.display(true)
p:set_srcrect{l = 0.5, t = 0.5, r = 0.8, b = 0.8}
a = p:get_srcrect().l * 2
font = cFont:new{width = 20,height = 50,facename = "ºÚÌå"}
t = cText:new{contents = "Ð¡ËÎÉµ¡Áthis is a demo", font = font}
alpha = p:get_alpha()
t:set_color(255, 0, 255)
function main()
	frame = frame + 1
	p:set_x(haha)
	p:set_y(math.sin(haha / 10) * 20 + 20)
	p:set_scalex(scalex)
	p:set_scaley(scalex)
	a, b, c = p:get_color()
	alpha = alpha + 1
	p:set_alpha(alpha)
	a = a + 20
	c = c + 10
	if input.key_down(input.IK_RIGHT) then
		haha = haha + 10
	end
	--sprite.set_color(p, a, b, c)
end
function destroy()
	p:destroy()
	t:destroy()
	aa:destroy()
	font:destroy()
end
