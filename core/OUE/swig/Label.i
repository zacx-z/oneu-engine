%module OUE
%include "Video.i"
%include "VideoNode.i"
%{
#include "../Label.h"
%}

namespace OneU
{
	enum { T_TOP = 0, T_BOTTOM = 4, T_VCENTER = 8};
	enum { T_LEFT = 0, T_RIGHT = 1, T_CENTER = 2 };
	
	class ILabel
		: public video::INode
	{
	public:
		virtual void setText(OneU::pcwstr text) = 0;
		virtual OneU::pcwstr getText() = 0;
		virtual void setColor(OneU::color_t color) = 0;
		virtual OneU::color_t getColor() = 0;
		virtual void setAlpha(OneU::ubyte alpha) = 0;
		OneU::ubyte getAlpha(){ return getColor().getAlpha(); }
		virtual void setAlign(OneU::uint32 align) = 0;//The align param could be form like this:'T_TOP + T_RIGHT'
	};
	
	%newobject Label_create;
	%rename(Label) Label_create;
	OneU::ILabel* Label_create(float Width, float Height, OneU::uint32 fontSize, OneU::pcwstr fontName = "Arial");
}
