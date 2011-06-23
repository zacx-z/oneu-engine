%module(directors = "1") OUE
%include "Video.i"
%include "VideoNode.i"
%{
#include "../Sprite.h"
%}
namespace OneU
{
	%rename(Sprite) ISprite;
	class ISprite
		: public OneU::video::INode
	{
	public:
		PROP_RW(getCenterX, setCenterX, ox);
		PROP_RW(getCenterY, setCenterY, oy);
		PROP_RW(getColor, setColor, color);
		PROP_RW(getAlpha, setAlpha, alpha);
		PROP_RW(getSrcRect, setSrcRect, srcRect);
		PROP_R(getWidth, width);
		PROP_R(getHeight, height);
		
		virtual void setImage(OneU::image_t img);
		virtual OneU::image_t getImage();
		virtual void setCenterX(float nx);
		virtual float getCenterX() const;
		virtual void setCenterY(float ny);
		virtual float getCenterY() const;

		virtual void setColor(OneU::color_t color) ;
		virtual OneU::color_t getColor();
		virtual void setAlpha(OneU::ubyte alpha);
		OneU::ubyte getAlpha();
		
		virtual void setSrcRect(rect rc);
		virtual rect getSrcRect();

		float getWidth();
		float getHeight();

		virtual void setBlendMode(OneU::video::BLENDMODE mode);
		virtual OneU::video::BLENDMODE getBlendMode();
		virtual void setColorBlendMode(OneU::uint32 mode);
		virtual OneU::uint32 getColorBlendMode();
	};
	
	%extend ISprite{
		ISprite(OneU::pcwstr file){
			return OneU::Sprite_create(OneU::GetVideo().loadImage(file));
		}
		ISprite(OneU::image_t& img){
			return Sprite_create(img);
		}
	};
}