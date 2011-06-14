%module(directors = "1") OUE
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
		virtual ~IScene();
	};
}

#ifdef SUPPORT_DIRECTORS
%director Scene;
%inline %{
class Scene : public OneU::IScene
{
public:
	virtual void update(){}
};
%}
#endif

%apply SWIGTYPE *VDISOWN {OneU::video::INode* child};
%inline %{
static void addToScene(OneU::video::INode* child){
	OneU::GetScene().getRenderScene()->addChild(child);
}
%}
%clear OneU::video::INode* child;
