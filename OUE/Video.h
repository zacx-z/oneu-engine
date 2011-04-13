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
		enum COLOR_BLENDMODE{ CBM_NONE = 0, CBM_MODULATE = 1, CBM_ADD = 2, 
			CBM_LEFT = 0x10, CBM_RIGHT = 0x20, CBM_TOP = 0x30, CBM_DOWN = 0x40 };

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 渲染节点
		 *
		 * 封装了一个渲染操作。
		 * 继承此类能扩充图形系统的功能。
		 */
		/* ----------------------------------------------------------------------------*/
		class INode
			: public Interface
		{
		private:
			friend class INodeContainer;
			INodeContainer* m_pParent;
			List<_NodeTag>::iterator m_It;//被parent所持有的容器的对应自己的迭代器
		public:
			bool visible;//visible为false时会停止update 不显示
			bool active;//active为false时会停止update
			INode() : m_pParent(NULL), visible(true), active(true) {}
			INodeContainer* getParent(){ return m_pParent; }
			~INode(){ detach(); }

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 使自身成为某个节点的儿子
			 * 
			 * @param parent 双亲节点
			 * @param z 深度
			 * @param tag 标签
			 *
			 * @remarks 生存期会被双亲管理
			 */
			/* ----------------------------------------------------------------------------*/
			inline void attach(INodeContainer* parent, int z = 0, pcwstr tag = NULL);//跟addChild功能一样
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 解除与双亲的关系
			 *
			 * @remarks 生存期不再被双亲管理
			 */
			/* ----------------------------------------------------------------------------*/
			inline void detach();

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 渲染函数
			 *
			 * 需要被重写来扩充。
			 * visible为true时被调用。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual void paint() = 0;

			//只有visible且active为true时才会被调用
			virtual void update(){}

			//可被重写
			virtual void describe(String& buffer, int depth){
				buffer.append(L"Unknown Node\n");
			}
			/**
			 * 在IVideo::render函数中被调用。
			 */
			void visit_paint(){
				if(visible)
					paint();
			}
			void visit_update(){
				if(visible && active)
					update();
			}
			void getDescription(String& buffer, int depth = 0){
				describe(buffer, depth);
			}
		};


		struct _NodeTag
		{
			int z;
			INode* child;
			String* tag;
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 渲染节点容器
		 *
		 * 即可以有孩子的渲染节点。
		 */
		/* ----------------------------------------------------------------------------*/
		class INodeContainer
			: public INode
		{
			friend void _DetachNodeFromParent(INode* node);
		protected:
			typedef List<_NodeTag> ListType;
			ListType m_Children;
			friend class INode;
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 添加孩子
			 * 
			 * @param child 孩子节点
			 * @param z 深度
			 * @param tag 标签
			 * 
			 * @returns 是否成功
			 * @remarks 孩子的生存期被自身管理
			 */
			/* ----------------------------------------------------------------------------*/
			bool addChild(INode* child, int z = 0, pcwstr tag = NULL){
				if(child->m_pParent != NULL){
					//GetGame().getBroadcast().message(tag == NULL ? L"Node Container fails to add Child." : String().format(L"Node Container fails to add Child, Tag:%s", tag))
					return false;//fail to add
				}
				ListType::iterator iter;
				for(iter = m_Children.begin(); iter != m_Children.end(); ++iter)
					if(iter->z > z)
						break;
				ListType::iterator it = m_Children.insert(iter);
				it->child = child;
				it->z = z;
				if(tag == NULL)it->tag = NULL;
				else it->tag = ONEU_NEW(String(tag));

				child->m_pParent = this;
				child->m_It = it;
				return true;
			}
			virtual void paint(){
				for(ListType::iterator it = m_Children.begin(); it != m_Children.end(); ++it){
					it->child->visit_paint();
				}
			}
			virtual void update(){
				for(ListType::iterator it = m_Children.begin(); it != m_Children.end(); ++it){
					it->child->visit_update();
				}
			}
			virtual void describe(String& buffer, int depth){
				buffer.append(L"Node Container\n");
				++depth;
				for(ListType::iterator it = m_Children.begin(); it != m_Children.end(); ++it){
					for(int i = 0; i < depth; ++i){
						buffer.append(L"\t");
					}
					it->child->getDescription(buffer, depth);
				}
			}
			virtual ~INodeContainer(){
				for(ListType::iterator it = m_Children.begin(); it != m_Children.end();){
					if(it->tag != NULL) ONEU_DELETE(it->tag);
					(it++)->child->destroy();//调用destroy时会使node detach，进而删除it所指元素。所以使用这种写法避免迭代器失效造成的问题。
				}
			}
		};

		inline void INode::attach(INodeContainer* parent, int z /* = 0 */, pcwstr tag /* = NULL */){
			parent->addChild(this, z, tag);
		}
		inline void INode::detach(){
			if(m_pParent != NULL){
				m_pParent->m_Children.erase(m_It);
				m_pParent = NULL;
			}
		}

		//渲染场景
		class IRenderScene
			: public INodeContainer
		{
		private:
			IRenderScene(){}
			friend class IVideo;
		public:
			void destroy(){ ONEU_DELETE(this); }
			void describe(String& buffer, int depth){
				buffer.append(L"<render scene>");
				INodeContainer::describe(buffer, depth);
			}
		};
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 图像（纹理）接口
		 */
		/* ----------------------------------------------------------------------------*/
		class IImage
			: public Interface
		{
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 添加引用计数
			 * 
			 * @returns 新的引用计数
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint addRef() = 0;
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 释放引用计数
			 *
			 * 如果引用计数释放后为0，则销毁图形。
			 * 
			 * @returns 新的引用计数
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint release() = 0;

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取图形宽度
			 * 
			 * @returns 宽度
			 * @remarks 并非纹理宽度，而是源文件的宽度。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint getWidth() = 0;
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取图形高度
			 * 
			 * @returns 高度
			 * @remarks 并非纹理高度，而是源文件的高度。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint getHeight() = 0;
		};
	}

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
	protected:
		video::IRenderScene* m_pRenderScene;
		video::INodeContainer* m_pRoot;
	public:
		IVideo() : m_pRenderScene(NULL), m_pRoot(NULL){}
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
		 * @brief 初始化系统
		 * 
		 * @param width 页面宽度
		 * @param height 页面高度
		 * @param bWindowed 是否窗口化
		 *
		 * @remarks 一般不需要客户调用
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void init(uint width, uint height, bool bWindowed) = 0;
		//if width or height为0 则尝试使用目前的设置
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 切换设备
		 *
		 * 该函数会释放所有资源并重建，是个昂贵的函数。
		 * 
		 * @param width 新页面宽度
		 * @param height 新页面高度
		 * @param bWindowed 是否窗口化
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void switchDevice(uint width, uint height, bool bWindowed) = 0;
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
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void update() = 0;
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
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 设置变换矩阵
		 *
		 * @param m 变换矩阵
		 */
		/* ----------------------------------------------------------------------------*/
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
		virtual void setBlendColor(dword mode, color_t color) = 0;
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
		video::IRenderScene* createRenderScene(){ return ONEU_NEW(video::IRenderScene);}
		//@}
		
		//others
		//不太重要的
		//向Shell输出该系统的信息
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

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 图像类型
	 *
	 * 可当做基本类型使用。一般从IVideo::loadImage返回的值直接用此类型变量持有。
	 */
	/* ----------------------------------------------------------------------------*/
	class image_t
	{
		video::IImage* m_pI;
	public:
		image_t(): m_pI(NULL){}
		image_t(video::IImage* i): m_pI(i){
			if(m_pI != NULL)
				m_pI->addRef();
		}
		image_t(const image_t& rhs): m_pI(rhs.m_pI){
			if(m_pI != NULL)
				m_pI->addRef();
		}
		~image_t(){
			if(m_pI != NULL)
				m_pI->release();
		}
		image_t& operator=(const image_t& rhs){
			if(m_pI != NULL)
				m_pI->release();
			m_pI = rhs.m_pI;
			m_pI->addRef();
			return *this;
		}
		image_t& operator=(video::IImage* i){
			if(m_pI != NULL)
				m_pI->release();
			m_pI = i;
			m_pI->addRef();
			return *this;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取图像接口
		 * 
		 * @returns 图像接口
		 */
		/* ----------------------------------------------------------------------------*/
		video::IImage* operator->(){ return m_pI; }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取图像接口
		 * 
		 * @returns 图像接口
		 */
		/* ----------------------------------------------------------------------------*/
		video::IImage* get(){ return m_pI; }
	};

}
