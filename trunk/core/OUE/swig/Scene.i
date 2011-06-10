%module OUE
%include "Video.i"

%{
#include "../Scene.h"
%}

namespace OneU
{
	class IScene
	{
	public:
		IScene();
		virtual void update() = 0;
		video::IRenderScene* getRenderScene();
	};
}
