%module OUE
%include "Video.i"
%include "VideoNode.i"
%{
#include "../Shape.h"
%}

namespace OneU
{
	class IShape
		: public OneU::video::INode
	{
	public:
		virtual void setColor(OneU::color_t color) = 0;
		virtual OneU::color_t getColor() = 0;

		virtual void setMode(bool bBorder) = 0;
	};

	%newobject Shape_rect;
	%rename(Rect) Shape_rect;
	OneU::IShape* Shape_rect(const OneU::rect& rc);
}
