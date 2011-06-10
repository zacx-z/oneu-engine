%module OUE
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
			bool visible;
			bool active;

			OneU::video::INodeContainer* getParent();

			%newobject detach;
			%extend {
			OneU::video::INode* detach(){
				self->OneU::video::INode::detach();
				return self;
			}
			}

			virtual OneU::pcwstr name();

			void create2DTransform();

			//@{
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
			//@}
		};

		class INodeContainer
			: public INode
		{
		public:
			virtual OneU::pcwstr name();
			
			%apply SWIGTYPE *VDISOWN {OneU::video::INode* child};
			bool addChild(OneU::video::INode* child, int z = 0, OneU::pcwstr tag = NULL);
			%clear OneU::video::INode* child;
		};
	}
}
