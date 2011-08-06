%module(directors = "1") OUE
%include "types.i"
%include "VideoNode.i"

%{
#include "../Video.h"
%}

namespace OneU
{
	namespace video
	{
		%nodefaultctor IRenderScene;
		class IRenderScene
			: public ILayer
		{};
		
		enum BLENDMODE{ BM_NORMAL = 0, BM_ADD };
		enum COLOR_BLENDMODE{ CBM_NONE = 0, CBM_ADD = 1, CBM_MODULATE = 2,
			CBM_LEFT = 0x10, CBM_RIGHT = 0x20, CBM_TOP = 0x30, CBM_DOWN = 0x40 };
		struct Mode
		{
			vector2u_t size;
			uint32 refreshRate;
		};
	}
	%rename(Image_t) image_t;
	class image_t
	{
	public:
		image_t();
	};
	
	%ListIns(OneU::video::Mode)
	
	%rename(Video) IVideo;
	class IVideo
	{
	public:
		IVideo();
		
		virtual OneU::pcwstr getName() = 0;

		virtual void prepare() = 0;
		virtual void init(OneU::uint32 width, OneU::uint32 height, bool bWindowed) = 0;
		
		virtual void switchDevice(OneU::uint32 width, OneU::uint32 height, bool bWindowed) = 0;
		virtual bool isWindowed() = 0;
		
		virtual OneU::vector2u_t getDeviceSize() = 0;
		virtual OneU::image_t loadImage(OneU::pcwstr filename) = 0;

		OneU::video::ILayer& getRoot();
		OneU::video::IRenderScene* getRenderScene();
		virtual OneU::video::IRenderScene* setRenderScene(OneU::video::IRenderScene* pRenderScene);
		inline OneU::video::IRenderScene* createRenderScene();
		
		virtual void showInfo() = 0;
		
		virtual void getAvailableMode(List<video::Mode>& OUTPUT) = 0;
	};
}