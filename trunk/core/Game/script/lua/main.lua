require "stdlib"

Atom.loadLib("../plugins/AtomDemo.at");
Waterfall = Atom.G.Waterfall

Scene = {}

frame = 0;

function Scene:init()
	self.a = Sprite("../picture.png")
	self.w = Waterfall();
end

function Scene:main()
	if Control:keyIsDown(Control.OIK_RIGHT) then
		self.a:setX(self.a:getX() + 10.0)
	end
	frame = frame + 1;
end

function Scene:destroy()
end
