require "OUE"

sp = OUE.Sprite("../picture.png");
OUE.addToScene(sp);
rc = OUE.Rect(OUE.rect(0, 0, 100, 100));
OUE.addToScene(rc);
label = OUE.Label(100, 100, 20);
label:setText("Hello World");
label:setX(400);
OUE.addToScene(label);
function mainloop()
end
