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
 * @brief ͼ��ϵͳ
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "OUEDefs.h"
#include "List.h"
#include "Vector.h"
#include "Matrix.h"

//IVideo Game�������֮һ
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
		 * @brief ��ɫ���ģʽ
		 *
		 * CBM_LEFT��ʾ��ɫֻ�����벿������CBM_MODULATE��λ������Ϊ��ʶ������ͬ����
		 *
		 * @sa IVideo::setBlendColor
		 */
		/* ----------------------------------------------------------------------------*/
		enum COLOR_BLENDMODE{ CBM_NONE = 0, CBM_ADD = 1, CBM_MODULATE = 2,
			CBM_LEFT = 0x10, CBM_RIGHT = 0x20, CBM_TOP = 0x30, CBM_DOWN = 0x40 };

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ͼ���������ӿ�
		 */
		/* ----------------------------------------------------------------------------*/
		class IImage
			: public InterfaceRef
		{
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief ��ȡͼ�ο���
			 * 
			 * @returns ����
			 * @remarks �����������ȣ�����Դ�ļ��Ŀ��ȡ�
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint32 getWidth() = 0;
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief ��ȡͼ�θ߶�
			 * 
			 * @returns �߶�
			 * @remarks ���������߶ȣ�����Դ�ļ��ĸ߶ȡ�
			 */
			/* ----------------------------------------------------------------------------*/
			virtual uint32 getHeight() = 0;
		};
	}

	/**
	 * @addtogroup group_game
	 */
	//@{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief ͼ��ϵͳ�ӿ�
	 * 
	 * @remarks ֻ֧�ֵ�Video�����ʹ���˶��Video������ʵ�ֻ�������⡣
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
		 * @brief ��ȡͼ��ϵͳ������
		 * 
		 * @returns ͼ��ϵͳ������
		 */
		/* ----------------------------------------------------------------------------*/
		virtual LPCTSTR getName() = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��ʼ��ϵͳ
		 * 
		 * @param width ҳ�����
		 * @param height ҳ��߶�
		 * @param bWindowed �Ƿ񴰿ڻ�
		 *
		 * @remarks һ�㲻��Ҫ�ͻ�����
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void init(uint32 width, uint32 height, bool bWindowed) = 0;
		//if width or heightΪ0 ����ʹ��Ŀǰ������
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief �л��豸
		 *
		 * �ú������ͷ�������Դ���ؽ����Ǹ�����ĺ�����
		 * 
		 * @param width ��ҳ�����
		 * @param height ��ҳ��߶�
		 * @param bWindowed �Ƿ񴰿ڻ�
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void switchDevice(uint32 width, uint32 height, bool bWindowed) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��Ⱦ
		 *
		 * ����ǰ��Ⱦ������Ⱦ����ǰ��ȾĿ���ϡ�
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void render() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief ���¶���
		*
		* ���µ�ǰ��Ⱦ������
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void update() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��ҳ
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void flip() = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ѯ�ʴ��ڻ�
		 * 
		 * @returns �Ƿ񴰿ڻ�
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool isWindowed() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��ȡ�豸ҳ�����
		 *
		 * ������@ref init��@ref switchDeviceָ���Ĳ�����
		 * 
		 * @returns ҳ���������
		 */
		/* ----------------------------------------------------------------------------*/
		virtual vector2u_t getDeviceSize() = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��ȡͼƬ
		 * 
		 * @param filename ͼƬ���ļ���
		 * 
		 * @returns ͼƬ�ӿڡ����ü���Ϊ0��һ��ֱ�Ӹ�ֵ��@ref image_t�����
		 *
		 * @attention ͼƬ�ӿڱ�����Video����ǰ���١�
		 */
		/* ----------------------------------------------------------------------------*/
		virtual image_t loadImage(pcwstr filename) = 0;

		/**
		 * @name ��Ⱦ����
		 * @{
		 */
		//���Կ��ǵ�����ȡRenderer��
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief ��Ⱦͼ��
		 *
		 * ��ͼ���Ծ��εķ�ʽ��Ⱦ��Ŀ�ꡣ
		 * ��������任��
		 * ��Ļ����ռ���D3DΪ׼����Ŀǰ��
		 *
		 * @param image ͼ��
		 * @param dest Ŀ��λ��
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void renderImage(video::IImage& image, const rect& dest = rect(0.0f, 0.0f, 1.0f, 1.0f)) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief ��Ⱦͼ��direct
		 *
		 * ��ͼ���Ծ��εķ�ʽ��Ⱦ��Ŀ�ꡣ
		 * ����������任��
		 * ��Ļ����ռ���D3DΪ׼����Ŀǰ��
		 *
		 * @param image ͼ��
		 * @param dest Ŀ��λ�á�������Ϊ��λ��
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void renderImage_d(video::IImage& image, const rect& dest) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief ���ñ任����
		 *
		 * @param m �任����
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void pushMatrix(const matrix& m) = 0;
		virtual void popMatrix(matrix* out = NULL) = 0;
		virtual matrix& _getTransform() = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief ����ͼ��Դ����
		 *
		 * ָʾ��Ⱦʱʹ��ͼ����һ���֡�
		 *
		 * @param src Դ���Ρ���һ����λ����1��ʾ�ҡ��£�0��ʾ���ϡ�Ŀǰ�����Ǵ���1�������
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void selectImageSourceRect(const rect& src = rect(0.0f, 0.0f, 1.0f, 1.0f)) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief ������ɫ���
		 *
		 * @param mode ���ģʽ��ʶ����ʶΪCOLORBLENDMODE������λ��
		 * @param color �����ɫ��alphaλ��Ч��Ϊ����ģʽ��
		 *
		 * @sa COLORBLENDMODE
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void setBlendColor(uint32 mode, color_t color) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief ���û��ģʽ
		 *
		 * @param mode ģʽ��ʶ
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void setBlendMode(video::BLENDMODE mode) = 0;
		/*@}*/

		/**
		 * @name ��Ⱦ������
		 * ����������һ�㲻���û�ʹ��
		 */
		//@{
		video::INodeContainer& getRoot(){ return *m_pRoot; }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��ȡ���Ⱦ����
		 * 
		 * @returns ��Ⱦ����
		 */
		/* ----------------------------------------------------------------------------*/
		video::IRenderScene* getRenderScene(){
			return m_pRenderScene;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ���û��Ⱦ����
		 * 
		 * @param pRenderScene �µ���Ⱦ����
		 * 
		 * @returns �ɵ���Ⱦ����
		 * @remarks �����ڲ�������
		 */
		/* ----------------------------------------------------------------------------*/
		virtual video::IRenderScene* setRenderScene(video::IRenderScene* pRenderScene){
			video::IRenderScene* ret = m_pRenderScene;
			m_pRenderScene = pRenderScene;
			return ret;
		}
		
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ������Ⱦ����
		 * 
		 * @returns �µ���Ⱦ����
		 */
		/* ----------------------------------------------------------------------------*/
		inline video::IRenderScene* createRenderScene();
		//@}
		
		//others
		//��̫��Ҫ��
		//��Shell�����ϵͳ����Ϣ
		virtual void showInfo() = 0;
	};

	//in game.cpp
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief ͼ��ϵͳ����
	 * 
	 * @returns ͼ��ϵͳ�ӿ�
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IVideo* Video_create();
	//@}

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief ͼ������
	 *
	 * �ɵ�����������ʹ�á�һ���IVideo::loadImage���ص�ֱֵ���ô����ͱ������С�
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
		 * @brief ��ȡͼ��ӿ�
		 * 
		 * @returns ͼ��ӿ�
		 */
		/* ----------------------------------------------------------------------------*/
		video::IImage* operator->(){ return m_pI; }
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief ��ȡͼ��ӿ�
		 * 
		 * @returns ͼ��ӿ�
		 */
		/* ----------------------------------------------------------------------------*/
		video::IImage* get(){ return m_pI; }
	};

}

#include "VideoNode.h"

namespace OneU
{
	namespace video
	{
		//��Ⱦ����
		class IRenderScene
			: public INodeContainer
		{
		private:
			IRenderScene(){}
			friend class IVideo;
		public:
			void describe(String& buffer, int depth){
				buffer.append(L"<render scene>");
				INodeContainer::describe(buffer, depth);
			}
		};
	}
	inline video::IRenderScene* IVideo::createRenderScene(){ return ONEU_NEW video::IRenderScene;}
}