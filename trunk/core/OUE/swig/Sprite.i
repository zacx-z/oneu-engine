%module(directors = "1") OUE
%include "Video.i"
%include "VideoNode.i"
%{
#include "../Sprite.h"
%}
namespace OneU
{
	class ISprite
		: public OneU::video::INode
	{
	public:
		virtual void setImage(OneU::image_t img) = 0;
		virtual OneU::image_t getImage() = 0;
		virtual void setCenterX(float nx) = 0;
		virtual float getCenterX() const = 0;
		virtual void setCenterY(float ny) = 0;
		virtual float getCenterY() const = 0;

		virtual void setColor(OneU::color_t color) = 0;
		virtual OneU::color_t getColor() = 0;
		virtual void setAlpha(OneU::ubyte alpha) = 0;
		OneU::ubyte getAlpha(){ return getColor().getAlpha(); }

		float getWidth();
		float getHeight();

		virtual void setBlendMode(OneU::video::BLENDMODE mode) = 0;
		virtual OneU::video::BLENDMODE getBlendMode() = 0;
		virtual void setColorBlendMode(OneU::uint32 mode) = 0;
		virtual OneU::uint32 getColorBlendMode() = 0;
	};
	%newobject Sprite_create;
	%rename(SpriteFromImage) Sprite_create;
	ISprite* Sprite_create(OneU::image_t& img);
}
%newobject Sprite;
%inline %{
static OneU::ISprite* Sprite(OneU::pcwstr file){
	return OneU::Sprite_create(OneU::GetVideo().loadImage(file));
}
%}