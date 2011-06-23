%module(directors = "1") OUE
%include "types.i"

%{
#include "../VideoNode.h"
%}


namespace OneU
{
	namespace video
	{
		%nodefaultctor INode;
		class INode
		{
		public:
			PROP_RW(getX, setX, x);
			PROP_RW(getY, setY, y);
			PROP_RW(getRotation, setRotation, rotation);
			PROP_RW(getScaleX, setScaleX, scaleX);
			PROP_RW(getScaleY, setScaleY, scaleY);
			
			bool visible;
			bool active;

			OneU::video::INodeContainer* getParent();

			%newobject detach;
			%extend {
			OneU::video::INode* detach(){
				if(!self->getParent()) return NULL;
				self->OneU::video::INode::detach();
				return self;//返回带有ownership的自身（原变量不含ownership）
			}
			}

			virtual OneU::pcwstr name();

			void create2DTransform();

			void setX(float x);
			float getX() const;
			void setY(float y);
			float getY() const;
			void setRotation(float z);
			float getRotation() const;
			void setScaleX(float ns);
			float getScaleX() const;
			void setScaleY(float ns);
			float getScaleY() const;
		};

		class INodeContainer
			: public INode
		{
		public:
			virtual OneU::pcwstr name();
			
			%apply SWIGTYPE *VDISOWN {OneU::video::INode* child};
			%apply SWIGTYPE *NONNULL {OneU::video::INode* child};
			bool addChild(OneU::video::INode* child, int z = 0, OneU::pcwstr tag = NULL);//child失去ownership 条件是child必须有ownership
			%clear OneU::video::INode* child;
		};
	}
}
