%module OUE
%{
#include "../OUE.h"
%}

%include "config.i"

%include "types.i"

%include "Game.i"

%include "Video.i"
%include "Stereo.i"
%include "Control.i"
%include "Scene.i"

%include "Sprite.i"
%include "Shape.i"
%include "Label.i"

%include "Event.i"//reserved
%include "Broadcast.i"//reserved
