require "graphics_ext"
require "input"
require "internal_input_word"
require "internal_user_manage"

sp = cSprite:new{file = "gameover.png"}
ef = cEffect:new(sp)
let = cSprite:new{file = "letters.png"}
function main()
	local newx = let:get_x() + input.get_mouse_lx()
	local newy = let:get_y() + input.get_mouse_ly()
	if(newx < 0) then newx = 0 end
	if(newx > renderer.width) then newx = 800 end
	if(newy < 0) then newy = 0 end
	if(newy > renderer.height) then newy = 600 end
	let:set_x(newx)
	let:set_y(newy)
end
function destroy()
	ef:destroy()
	sp:destroy()
end
