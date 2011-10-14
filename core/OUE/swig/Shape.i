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
		PROP_RW(getMode, setMode, mode);
		
		virtual void setColor(OneU::color_t color);
		virtual OneU::color_t getColor();

		virtual void setMode(bool bBorder);
		virtual bool getMode();
		
	};

	%extend IShape{
		IShape(const OneU::rect& rc){
			return Shape_rect(rc);
		}
	};
}
