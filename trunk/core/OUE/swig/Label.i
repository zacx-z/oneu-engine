%module(directors = "1") OUE
%include "Video.i"
%include "VideoNode.i"
%{
#include "../Label.h"
%}

namespace OneU
{
	enum { T_TOP = 0, T_BOTTOM = 4, T_VCENTER = 8};
	enum { T_LEFT = 0, T_RIGHT = 1, T_CENTER = 2 };
	
	%rename (Label) ILabel;
	class ILabel
		: public video::INode
	{
	public:
		PROP_RW(getAlpha, setAlpha, alpha);
		PROP_RW(getText, setText, text);
		PROP_RW(getColor, setColor, color);
		PROP_RW(getAlign, setAlign, align);
		
		virtual void setText(OneU::pcwstr text);
		virtual OneU::pcwstr getText();
		virtual void setColor(OneU::color_t color);
		virtual OneU::color_t getColor();
		virtual void setAlpha(OneU::ubyte alpha);
		OneU::ubyte getAlpha();
		virtual void setAlign(OneU::uint32 align);
		virtual uint32 getAlign();
	};
	
	%extend ILabel{
		ILabel(float Width, float Height, OneU::uint32 fontSize, OneU::pcwstr fontName){
			return OneU::Label_create(Width, Height, fontSize, fontName);
		}
		ILabel(float Width, float Height, OneU::uint32 fontSize){
			return OneU::Label_create(Width, Height, fontSize, L"Arial");
		}
	};
	
}
