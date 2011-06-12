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
* @file VideoNode.h
* @brief 渲染结点
* @author Ladace
* @version 1.0.0.1
* @date 2011-05-31
*/
#pragma once
#include "Video.h"

namespace OneU
{
	inline IVideo& GetVideo();
	namespace video
	{
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

			struct _TransformInfo2D{
				matrix m;
				bool mIsReady;
				float x, y;
				float scalex, scaley;
				float rotz;
			};
			union{
				_TransformInfo2D* _2;
				//why union? - reserved.
			}Transform;

		public:
			/**
			 * @brief 可视性
			 *
			 * 当它为false时会停止update，并且不显示。
			 */
			bool visible;
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 活动性
			 *
			 * 当它为false时会停止update。
			 */
			/* ----------------------------------------------------------------------------*/
			bool active;
			INode() : m_pParent(NULL), visible(true), active(true){memset(&Transform, 0, sizeof(Transform)); }

			~INode(){
				detach();
				if(Transform._2)
					ONEU_DELETE_T(Transform._2);
			}


			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 获取父结点
			 *
			 * @remarks 若没有父亲，则返回NULL。
			 */
			/* ----------------------------------------------------------------------------*/
			INodeContainer* getParent(){ return m_pParent; }

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
			 * 需要被重写来扩充，填充上渲染该结点时的渲染操作。
			 * @remarks visible为true时被调用。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual void paint() = 0;

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 更新函数
			 *
			 * 需要被重写来扩充，填充上更新结点时的操作。
			 * @remarks 只有visible且active为true时才会被调用。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual void update(){}

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 名字函数
			 *
			 * 可重写来为改变该结点的名称，使其易于测试。
			 * @returns 该节点的名字
			 */
			/* ----------------------------------------------------------------------------*/
			virtual pcwstr name(){
				return L"Unknown Node";
			}

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 描述函数
			 *
			 * 可重写来更改对该结点描述的内容。
			 * @remarks 默认调用name函数描述该结点，如果有子节点再将其一一描述。一般不需要客户重写。
			 */
			/* ----------------------------------------------------------------------------*/
			virtual void _describe(String& buffer, int depth){
				buffer.append(name());
				buffer.append(L"\n");
			}


			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 渲染
			 * @remarks 在IVideo::render函数中被调用。
			 */
			/* ----------------------------------------------------------------------------*/
			void visit_paint(){
				if(visible){
					bool b = _beginTransform();
					paint();
					if(b) GetVideo().popMatrix();
				}
			}

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 更新
			 * @remarks 在IVideo::update函数中被调用。
			 */
			/* ----------------------------------------------------------------------------*/
			void visit_update(){
				if(visible && active)
					update();
			}
			void getDescription(String& buffer, int depth = 0){
				_describe(buffer, depth);
			}


			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 创建2D变换
			 *
			 * 为节点创建2D变换。在绘制时会自动进行变换（将变换矩阵压入矩阵栈）。
			 * @remarks 创建变换后@ref setX等函数可用。
			 */
			/* ----------------------------------------------------------------------------*/
			void create2DTransform(){
				if(Transform._2) return;
				Transform._2 = ONEU_NEW_T(_TransformInfo2D);
				memset(Transform._2, 0, sizeof(_TransformInfo2D));
				Transform._2->scalex = Transform._2->scaley = 1.0f;
			}

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 开始变换
			 *
			 * 如果有变换矩阵，则将矩阵压入矩阵栈。
			 * @remarks 绘制时调用。
			 */
			/* ----------------------------------------------------------------------------*/
			bool _beginTransform(){
				if(Transform._2){
					if(!Transform._2->mIsReady){
						Transform._2->mIsReady = true;
						Transform._2->m = 
							matrix().setScale(vector3(Transform._2->scalex, Transform._2->scaley, 1.0f))
							* matrix().setRotation(vector3(0.0f, 0.0f, Transform._2->rotz))
							* matrix().setTranslation(vector3(Transform._2->x, Transform._2->y, 0.0f));
					}
					GetVideo().pushMatrix(Transform._2->m);
					return true;
				}
				return false;
			}

			/**
			 * @name 变换属性操作函数
			 */
			//@{
			void setX(float x){
				if(Transform._2){
					Transform._2->x = x;
					Transform._2->mIsReady = false;
				}else ONEU_ASSERT(FALSE);
			}
			float getX() const{
				if(Transform._2){
					return Transform._2->x;
				}else ONEU_ASSERT(FALSE);
				return 0.0f;
			}
			void setY(float y){
				if(Transform._2){
					Transform._2->y = y;
					Transform._2->mIsReady = false;
				}else ONEU_ASSERT(FALSE);
			}
			float getY() const{
				if(Transform._2){
					return Transform._2->y;
				}else ONEU_ASSERT(FALSE);
				return 0.0f;
			}
			void setRotation(float z){
				if(Transform._2){
					Transform._2->rotz = z;
					Transform._2->mIsReady = false;
				}else ONEU_ASSERT(FALSE);
			}
			float getRotation() const{
				if(Transform._2){
					return Transform._2->rotz;
				}else ONEU_ASSERT(FALSE);
				return 0.0f;
			}
			void setScaleX(float ns){
				if(Transform._2){
					Transform._2->scalex = ns;
					Transform._2->mIsReady = false;
				}else ONEU_ASSERT(FALSE);
			}
			float getScaleX() const{
				if(Transform._2){
					return Transform._2->scalex;
				}else ONEU_ASSERT(FALSE);
				return 0.0f;
			}
			void setScaleY(float ns){
				if(Transform._2){
					Transform._2->scaley = ns;
					Transform._2->mIsReady = false;
				}else ONEU_ASSERT(FALSE);
			}
			float getScaleY() const{
				if(Transform._2){
					return Transform._2->scaley;
				}else ONEU_ASSERT(FALSE);
				return 0.0f;
			}
			//@}
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
			virtual ~INodeContainer(){
				for(ListType::iterator it = m_Children.begin(); it != m_Children.end();){
					if(it->tag != NULL) ONEU_DELETE_T(it->tag);
					ONEU_DELETE (it++)->child;//调用destroy时会使node detach，进而删除it所指元素。所以使用这种写法避免迭代器失效造成的问题。
				}
			}
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
				else it->tag = ONEU_NEW_T(String(tag));

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
			virtual pcwstr name(){
				return L"NodeContainer";
			}
			virtual void _describe(String& buffer, int depth){
				INode::_describe(buffer, depth);
				++depth;
				for(ListType::iterator it = m_Children.begin(); it != m_Children.end(); ++it){
					for(int i = 0; i < depth; ++i){
						buffer.append(L"\t");
					}
					it->child->getDescription(buffer, depth);
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
	}
}