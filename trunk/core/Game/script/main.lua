require "OUE"
sp = OUE.Sprite_create(OUE.GetVideo():loadImage("../picture.png"));
OUE.GetScene():getRenderScene():addChild(sp);
function main()
	sp:setX(sp:getX() + 1);
end
