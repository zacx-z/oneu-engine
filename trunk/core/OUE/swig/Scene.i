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
		PROP_R(getRenderScene, RS);
		IScene();
		virtual void update() = 0;
		video::IRenderScene* getRenderScene();
		virtual ~IScene();
		
		OneU::IInputReceiver* replaceInputFocus(OneU::IInputReceiver* pIR);
		void pushInputFocus(OneU::IInputReceiver* pIR);
		OneU::IInputReceiver* popInputFocus();
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

#ifdef SWIGLUA
%inline %{
static void addToScene(OneU::video::INode* child, int z = 0, OneU::pcwstr tag = NULL){
	OneU::GetScene().getRenderScene()->addChild(child, z, tag, false);
}
%}
#endif