require "Atom"

cSprite = Atom.S["Sprite"]
Control = Atom.S["Control"]
Atom.loadLib("Rocker.at");
cWaterfall = Atom.S["Waterfall"]

Scene = {}

frame = 0;

function Scene:init()
	self.a = cSprite("../picture.png", -100)
	self.w = cWaterfall();
end

function Scene:main()
	if Control:keyIsDown(Control.OIK_RIGHT) then
		self.a:setX(self.a:getX() + 10.0)
	end
	frame = frame + 1;
end
