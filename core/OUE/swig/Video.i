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
			: public INodeContainer
		{};
		
		enum BLENDMODE{ BM_NORMAL = 0, BM_ADD };
		enum COLOR_BLENDMODE{ CBM_NONE = 0, CBM_ADD = 1, CBM_MODULATE = 2,
			CBM_LEFT = 0x10, CBM_RIGHT = 0x20, CBM_TOP = 0x30, CBM_DOWN = 0x40 };
	}
	%rename(Image_t) image_t;
	class image_t
	{
	public:
		image_t();
		image_t(const image_t& rhs);
		image_t& operator=(const image_t& rhs);
		image_t& operator=(video::IImage* i);
	};
	
	%rename(Video) IVideo;
	class IVideo
	{
	public:
		IVideo();
		
		virtual LPCTSTR getName() = 0;

		virtual void init(OneU::uint32 width, OneU::uint32 height, bool bWindowed) = 0;
		
		virtual void switchDevice(OneU::uint32 width, OneU::uint32 height, bool bWindowed) = 0;
		virtual bool isWindowed() = 0;
		
		virtual OneU::vector2u_t getDeviceSize() = 0;
		virtual OneU::image_t loadImage(OneU::pcwstr filename) = 0;

		OneU::video::INodeContainer& getRoot();
		OneU::video::IRenderScene* getRenderScene();
		virtual OneU::video::IRenderScene* setRenderScene(OneU::video::IRenderScene* pRenderScene);
		inline OneU::video::IRenderScene* createRenderScene();
		
		virtual void showInfo() = 0;
	};
}