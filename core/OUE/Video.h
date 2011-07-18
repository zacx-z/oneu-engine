/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**
 * @file Video.h
 * @brief 图形系统
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "List.h"
#include "Vector.h"
#include "Matrix.h"
#include "TplTypes.h"

//IVideo Game基本组成之一
namespace OneU
{
	namespace video
	{
		struct _NodeTag;
		class INode;
		class INodeContainer;

		class IRenderScene;
		class IImage;
		class IModule;

		enum BLENDMODE{ BM_NORMAL = 0, BM_ADD };
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 颜色混合模式
		 *
		 * CBM_LEFT表示颜色只混合左半部，可与CBM_MODULATE等位或来作为标识。其它同理。
		 *
		 * @sa IVideo::setBlendColor
		 */
		/* ----------------------------------------------------------------------------*/
		enum COLOR_BLENDMODE{ CBM_NONE = 0, CBM_ADD = 1, CBM_MODULATE = 2,
			CBM_LEFT = 0x10, CBM_RIGHT = 0x20, CBM_TOP = 0x30, CBM_DOWN = 0x40 };

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 图像（纹理）接口
		 */
		/* ----------------------------------------------------------------------------*/
		class IImage
			: public InterfaceRef
		{
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取图形宽度
			 * 
			 * @returns 宽度
			 * @remarks 并非纹理宽度，而是源文件的宽度。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint32 getWidth() = 0;
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取图形高度
			 * 
			 * @returns 高度
			 * @remarks 并非纹理高度，而是源文件的高度。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint32 getHeight() = 0;
		};

		struct Mode
		{
			vector2u_t size;
			uint32 refreshRate;
		};
	}

	typedef RefWrapper<video::IImage> image_t;
	/**
	 * @addtogroup group_game
	 */
	//@{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 图形系统接口
	 * 
	 * @remarks 只支持单Video。如果使用了多个Video，可能实现会出现问题。
	 */
	/* ----------------------------------------------------------------------------*/
	class IVideo
		: public Interface
	{
		friend class video::IModule;
		List<video::IModule*> m_ModuleList;
	protected:
		video::IRenderScene* m_pRenderScene;
		video::INodeContainer* m_pRoot;
	public:
		IVideo() : m_pRenderScene(NULL), m_pRoot(NULL){}
		inline ~IVideo();
		/* ----------------------------------------------------------------------------*/
		/**
		* @brief 注册模块
		*
		* @param module 模块地址
		* @remarks 一般不需客户直接调用。
		*/
		/* ----------------------------------------------------------------------------*/
		List<video::IModule*>::iterator __addModule(video::IModule* module);
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取图形系统的名称
		 * 
		 * @returns 图形系统的名称
		 */
		/* ----------------------------------------------------------------------------*/
		virtual LPCTSTR getName() = 0;

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 准备系统
		 *
		 * @remarks 调用此函数后可以进行检查设备参数的操作。
		 * 若不检查设备参数，也可以不调用该函数，在init函数会自动调用该函数。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void prepare() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 初始化系统
		 * 
		 * @param width 页面宽度
		 * @param height 页面高度
		 * @param bWindowed 是否窗口化
		 *
		 * @remarks 一般不需要客户调用
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void init(uint32 width, uint32 height, bool bWindowed) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 切换设备
		 *
		 * 该函数会释放所有资源并重建，是个昂贵的函数。
		 * 
		 * @param width 新页面宽度
		 * @param height 新页面高度
		 * @param bWindowed 是否窗口化
		 * @remarks 如果width或者height为0，则尝试使用目前的设置。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void switchDevice(uint32 width, uint32 height, bool bWindowed) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 渲染
		 *
		 * 将当前渲染场景渲染到当前渲染目标上。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void render() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 更新动画
		*
		* 更新当前渲染场景。
		* @param dt 时间长度
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void update(float dt) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 翻页
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void flip() = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 询问窗口化
		 * 
		 * @returns 是否窗口化
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool isWindowed() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取设备页面宽高
		 *
		 * 宽高是@ref init和@ref switchDevice指定的参数。
		 * 
		 * @returns 页面宽高向量
		 */
		/* ----------------------------------------------------------------------------*/
		virtual vector2u_t getDeviceSize() = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 读取图片
		 * 
		 * @param filename 图片的文件名
		 * 
		 * @returns 图片接口。引用计数为0。一般直接赋值在@ref image_t变量里。
		 *
		 * @attention 图片接口必须在Video销毁前销毁。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual image_t loadImage(pcwstr filename) = 0;

		/**
		 * @name 渲染函数
		 * @{
		 */
		//可以考虑单独提取Renderer类
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 渲染图像
		 *
		 * 将图像以矩形的方式渲染到目标。
		 * 经过坐标变换。
		 * 屏幕坐标空间以D3D为准。（目前）
		 *
		 * @param image 图像
		 * @param dest 目标位置
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void renderImage(video::IImage& image, const rect& dest = rect(0.0f, 0.0f, 1.0f, 1.0f)) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 渲染图像direct
		 *
		 * 将图像以矩形的方式渲染到目标。
		 * 不经过坐标变换。
		 * 屏幕坐标空间以D3D为准。（目前）
		 *
		 * @param image 图像
		 * @param dest 目标位置。以像素为单位。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void renderImage_d(video::IImage& image, const rect& dest) = 0;
		virtual void setTransform(const matrix& m) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设置图形源矩形
		 *
		 * 指示渲染时使用图形哪一部分。
		 *
		 * @param src 源矩形。归一化单位。即1表示右、下，0表示左、上。目前不考虑大于1的情况。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void selectImageSourceRect(const rect& src = rect(0.0f, 0.0f, 1.0f, 1.0f)) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设置颜色混合
		 *
		 * @param mode 混合模式标识。标识为COLORBLENDMODE或它的位或。
		 * @param color 混合颜色。alpha位有效且为调制模式。
		 *
		 * @sa COLORBLENDMODE
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void setBlendColor(uint32 mode, color_t color) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设置混合模式
		 *
		 * @param mode 模式标识
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void setBlendMode(video::BLENDMODE mode) = 0;
		/*@}*/

		/**
		 * @name 渲染树函数
		 * 这三个函数一般不被用户使用
		 */
		//@{
		video::INodeContainer& getRoot(){ return *m_pRoot; }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取活动渲染场景
		 * 
		 * @returns 渲染场景
		 */
		/* ----------------------------------------------------------------------------*/
		video::IRenderScene* getRenderScene(){
			return m_pRenderScene;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 设置活动渲染场景
		 * 
		 * @param pRenderScene 新的渲染场景
		 * 
		 * @returns 旧的渲染场景
		 * @remarks 生存期不被管理
		 */
		/* ----------------------------------------------------------------------------*/
		virtual video::IRenderScene* setRenderScene(video::IRenderScene* pRenderScene){
			video::IRenderScene* ret = m_pRenderScene;
			m_pRenderScene = pRenderScene;
			return ret;
		}
		
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 创建渲染场景
		 * 
		 * @returns 新的渲染场景
		 */
		/* ----------------------------------------------------------------------------*/
		inline video::IRenderScene* createRenderScene();
		//@}

		/**
		 * @name 设备检查函数
		 * @remarks 调用前要调用prepare函数或调用init函数。
		 */
		//@{
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取可用的显示模式
		 * 
		 * @param buf 将显示模式作为列表返回。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void getAvailableMode(List<video::Mode>& buf) = 0;
		//@}
		//others
		//不太重要的
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 输出信息
		 *
		 * 向Shell输出该系统的信息
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void showInfo() = 0;
	};

	//in game.cpp
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 图形系统工厂
	 * 
	 * @returns 图形系统接口
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IVideo* Video_create();
	//@}
}

#include "VideoNode.h"

namespace OneU
{
	namespace video
	{

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 渲染场景
		 *
		 * @remarks 每一个场景对应一个渲染场景。
		 */
		/* ----------------------------------------------------------------------------*/
		class IRenderScene
			: public INodeContainer
		{
		private:
			IRenderScene(){}
			friend class IVideo;
		public:
			void _describe(String& buffer, int depth){
				buffer.append(L"<render scene>");
				INodeContainer::_describe(buffer, depth);
			}
		};
	}
	inline video::IRenderScene* IVideo::createRenderScene(){ return ONEU_NEW video::IRenderScene;}

	namespace video
	{
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 图形子模块
		 * 
		 * 可继承来实现图形系统的子模块。
		 * @remarks 子模块为单件，在建立后会注册到图形系统里，会在图形系统销毁前自动销毁。
		 */
		/* ----------------------------------------------------------------------------*/
		class IModule
			: public Interface
		{
			friend class IVideo;
			List<IModule*>::iterator m_ID;
		public:
			IModule(){
				m_ID = GetVideo().__addModule(this);
			}
			~IModule(){
				GetVideo().m_ModuleList.erase(m_ID);
			}
		};
	}
	inline IVideo::~IVideo(){
		for(List<video::IModule*>::iterator it = m_ModuleList.begin(); it != m_ModuleList.end();)//销毁所有模块
			ONEU_DELETE *(it++);
	}
	inline List<video::IModule*>::iterator IVideo::__addModule(video::IModule* module){
		m_ModuleList.pushBack(module);
		return m_ModuleList.end().prev();
	}
}