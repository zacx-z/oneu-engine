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

%feature("director") OneU::IScene;


%apply SWIGTYPE *VDISOWN {OneU::video::INode* child};
%inline %{
void addToScene(OneU::video::INode* child){
	OneU::GetScene().getRenderScene()->addChild(child);
}
%}
%clear OneU::video::INode* child;
