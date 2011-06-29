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

			void detach();

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
			
			%apply SWIGTYPE *NONNULL {OneU::video::INode* child};
			%extend{
				bool addChild(OneU::video::INode* child, int z = 0, OneU::pcwstr tag = NULL){
					return self->addChild(child, z, tag, false);//²»×ªÒÆownership
				}
			}
			%clear OneU::video::INode* child;
		};
	}
}
