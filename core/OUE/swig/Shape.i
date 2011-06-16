%module(directors = "1") OUE
%include "Video.i"
%include "VideoNode.i"
%{
#include "../Shape.h"
%}

namespace OneU
{
	%rename(Shape) IShape;
	class IShape
		: public OneU::video::INode
	{
	public:
		PROP_RW(getColor, setColor, color);
		
		virtual void setColor(OneU::color_t color);
		virtual OneU::color_t getColor();

		virtual void setMode(bool bBorder);
		
	};

	%extend IShape{
		IShape(const OneU::rect& rc){
			return Shape_rect(rc);
		}
	};
}
