//此文件仅用于生成文档

namespace OneU{
/* ----------------------------------------------------------------------------*/
/** 
 * @mainpage 万有引擎2D技术文档
 *
 * This ain't a game. This is One Universe.\n
 * \n
 * 万有引擎2D最终目的是实现跨平台支持OpenGL和DirectX并能搭载扩展插件的一个2D游戏平台。\n
 * \n
 *
 * 在SDK包里共包含：
 *  - c++工程SDK（仅限于VC使用）。
 *  - 单场景采用解释器的游戏执行包。作为该引擎的简化执行程序，可以通过编写脚本写游戏。
 *  - 文档。
 *  .
 * @section section_contents 文档结构
 *  - @subpage page_cpp
 *  - @subpage page_lua
 *  - @subpage page_extending
 *  - 参考
 *		- @subpage page_run
 *		.
 *  .
 * @section section_faq FAQ
 * Q: 为什么SDK只能使用VC？\n
 * A: 因为编译器之间的对象内存布局不兼容，该SDK是使用VC编译的，如果使用其他编译器可能会导致程序崩溃。所幸的是，VC的不同版本在对象内存布局保持了一致性。因此对VC的版本没有限制。以后会推出MinGW版的SDK。\n
 * Q: 可否在openGL和DX内核切换？\n
 * A: 现在只做了DX内核。\n
 * Q: 可以用脚本来实现配置？
 * A: 这是Atom的功能之一。因为Atom是先于其他组件创建的，
 */

/* ----------------------------------------------------------------------------*/
/**
 * @page page_cpp 使用C++ SDK
 * 该SDK包含了游戏具备的底层元素，可以在其上建立游戏框架编写游戏，也可以用于扩展引擎。\n
 *
 * 一个使用万有引擎的HelloWorld，参考HelloWorld.cpp。\n
 *
 *  - @ref section_defintion
 *		- @ref section_renderscene
 *		.
 *  - @ref section_program
 *		- @ref section_config_sdk
 *		- @ref section_start
 *		- @ref section_display_image
 *		- @ref section_retrieve_control
 *		- @ref section_user_video_node
 *		- @ref section_to_extend
 *		.
 *  .
 * @section section_defintion 概念
 * 主要接口：
 *  - 游戏接口 @ref IGame\n
 *  管理整个游戏程序流程，负责配置和创建、刷新各子系统。最顶级接口。
 *  - 广播接口 @ref IBroadcast\n
 *  处理全局事件。
 *  - 图形接口 @ref IVideo
 *  负责显示和管理图像、动画。
 *  - 控制接口 @ref IControl\n
 *  负责处理键盘和鼠标等输入设备。
 *  - 场景接口 @ref IScene\n
 *  负责处理游戏逻辑。一个场景是一个逻辑单元，被游戏对象所持有和处理。
 * .
 * 
 * @subsection section_event 事件系统
 * 事件系统由事件派发器，监听器和事件（包括事件参数）组成。使用字符串来区分一类事件。事件派发器是接受事件的类，通过向事件派发器添加监听器来处理某一个事件。监听器可添加多个。\n
 * 目前的事件机制没有采用队列，是在发送消息的同时执行监听器的触发函数。\n
 * 公共事件会通过广播系统传播。广播类是事件派发器的继承类。\n
 * 键盘事件：调用@ref IGame::pushInputFocus将一个事件派发器设为输入焦点，该事件派发器可以接收到键盘的输入事件。
 * @subsection section_renderscene 图形系统和渲染场景
 * 万有引擎使用树来渲染。树中的每一个节点即表示一个渲染操作。渲染时会对树进行遍历。\n
 * 一个渲染场景即为一个渲染树。一般而言，一个场景对应一个渲染场景。\n
 * @ref video::INode即是渲染节点的接口，@ref video::INodeContainer是允许有孩子的渲染节点接口。z值表示一个渲染节点在其兄弟节点被渲染的顺序，z值小的会先被渲染。\n
 * 继承这两个类可以扩充图形系统的功能。\n
 *
 * @section section_program 编写程序
 * @subsection section_config_sdk 配置SDK
 *  - 添加其include文件夹添加到VC的include路径里或将其拷贝到工程内，链接libs/x86文件夹下的两个lib文件。\n
 *  - 拷贝Libs下两个dll到工程内。以VC2005为例，工程名为Client，则拷贝到Client/Client下。
 *  - 若使用脚本，拷贝script文件夹（脚本库）到游戏目录下。
 *  .
 * @subsection section_start 开始
 * 首先继承@ref IScene，重写update函数。
 * 编写主函数，主函数标准流程：
 *  - 调用@ref Game_build函数建立Game单态。
 *  - 调用@ref GetGame获取IGame接口。调用@ref IGame::init函数初始化游戏。
 *  - 调用@ref IGame接口配置游戏逻辑等。调用@ref IGame::replaceScene创建一个场景并作为游戏活动场景。
 *  - 调用@ref IGame::run，返回其返回值。该函数会自动进行游戏流程。
 *  .
 * 首先定义一个场景：
 * @code
 * using namespace OneU;
 * class HelloScene
 *	: public IScene
 * {
 * public:
 *	HelloScene(){
 *	}
 *	void update(){
 *	}
 * };
 * @endcode
 * 接下来写主函数：
 * @code
 * extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
 *	Game_build(Game_create);
 *	IGame* game = GetGame();
 *	game->init(L"Hello World", 800, 600, true);
 *	game->replaceScene(ONEU_NEW HelloScene);
 *	return game->run();
 * }
 * @endcode
 * 编译运行后是一个空窗口。
 * @subsection section_display_image 显示图像
 * 重新定义HelloScene，在构造函数里创建精灵:
 * @code
 * class HelloScene
 *	: public IScene
 * {
 *	ISprite* m_pS;
 * public:
 *	HelloScene(){
 *		m_pS = Sprite_create(GetVideo.loadImage(L"picture.png"));
 *		getRenderScene().addChild(m_pS, 0);
 *	}
 *	void update(){
 *	}
 * };
 * @endcode
 * 在根目录下添加picture.png图形文件。启动游戏，picture.png显示在窗口的左上角。\n
 * @ref ISprite是引擎内置的精灵接口。调用Sprite_create创建精灵。\n
 * 调用@ref IScene::getRenderScene获得该场景对应的渲染场景，将精灵节点加入该场景，即可使其显示出来。\n
 * @subsection section_retrieve_control 响应输入
 * 响应输入，一种办法是直接访问输入设备，通过查看状态来响应该输入。\n
 * 在update函数添加如下代码：
 * @code
 * if(GetControl().keyIsDown(OIK_RIGHT))
 *	m_pS->setX(m_pS->getX() + 10.0f);
 * @endcode
 * 编译运行，按下方向键右键，图像向右移动。\n
 * @ref IScene::update函数在游戏的每一帧都会调用一次。\n
 * @ref GetControl函数获得控制器对象，可用来查看键盘状态。OIK_RIGHT是方向键右键对应的扫描码。\n
 * 精灵使用的坐标系以左上角为原点，像素为单位。\n
 * @subsection section_user_video_node 自定义渲染元素
 * 渲染元素指渲染树中的结点或子树。\n
 * 引擎内置的几种渲染元素（Sprite, Label等）比较简单，是远远不够用的。该引擎提供一套自己定义渲染元素的方法，可以有效的扩展出开发者想要的功能。\n
 * 以BlinkSprite为例，讲述如何编写新的渲染元素。
 * @code
 * class BlinkSprite
 *	: public video:INode
 * {
 *	image_t m_Img;
 * public:
 *	BlinkSprite(image_t& img)
 *		: m_Img(img)
 *	void paint(){
 *		IVideo& Vi = GetVideo();
 *		Vi.setBlendColor(video::CBM_NONE, 0);
 *		Vi.setBlendMode(video::BM_NORMAL);
 *		Vi.renderImage_d(*m_Img.get(), rect(0, 0, 100, 100));
 *	}
 * };
 * @endcode
 * 试试在HelloScene的构造函数里加入如下代码：
 * @code
 * getRenderScene().addChild(ONEU_NEW BlinkSprite(GetVideo().loadImage(L"picture.png")), 1);
 * @endcode
 * 编译运行，picture.png显示在左上角100 * 100的矩形内。\n
 * 重写@ref video::INode::paint函数，在函数里编写渲染元素渲染时的所需的操作，当INode的成员visible为true时，该函数会在每一帧被调用。\n
 * @ref IVideo::setBlendColor等是渲染函数，只被允许在渲染期间调用，即只能在@ref video::INode::paint函数里调用。\n
 * 接下来要该精灵能够闪烁。\n
 * 为BlinkSprite添加如下成员：
 * @code
 * bool m_bV;
 * @endcode
 * 在构造函数里初始化为true。\n
 * 修改paint函数，重写INode::update函数：\n
 * @code
 *	void paint(){
 *		if(!m_bV) return;
 *		IVideo& Vi = GetVideo();
 *		Vi.setBlendColor(video::CBM_NONE, 0);
 *		Vi.setBlendMode(video::BM_NORMAL);
 *		Vi.renderImage_d(*m_Img.get(), rect(0, 0, 100, 100));
 *	}
 * void update(){
 *	m_bV = !m_bV;
 * }
 * @endcode
 * 编译运行，图像会每隔一帧闪烁一次。\n
 * @ref video::INode::update函数是在visible和active均为true时，每一帧都会调用的函数，用于刷新动画。\n
 * @subsection section_to_extend 进阶：扩展引擎
 * 在扩展引擎之前，请确保首先阅读了@ref page_principles并了解了必须遵守的原则。
 * 参见@ref page_extending。
 */
/**
 * @page author_backup 作者备份
 * @section section_layer 代码分层
 * 文件及功能的层级关系。为了避免混乱以及无意识的间接递归，有必要将底层代码分层，使得低层次的不能调用高层次的函数。\n
 * 层次由低到高：
 *  - 基本定义（typedef）。
 *  - IAllocator，Memory。
 *  - String。（String对于分配内存失败的默认处理是不做任何操作）
 *  - Logger。
 *  - Error。错误处理
 *  - 各种容器及在其上建立的构架。
 *  -.
 *
 */

/* ----------------------------------------------------------------------------*/
/**
 * @page page_principles 编程原则
 * @section section_interface_principles dll库编写原则
 * dll库是能够与其他C程序链接的库。我们可以将代码编译成dll并提供头文件供其它dll或exe使用。\n
 * 制作dll库的时候，为了能够被其他程序正确使用，有些规则必须遵守。\n
 * dll库其中的一些头文件是作为库的头文件的，它是别的代码使用这个库的根本。这些头文件只能出现如下定义（只有这些定义才能被外部程序正确使用）：
 *  - C式的全局导出函数。使用ONEU_API宏使一个函数成为C式的导出函数。如ONEU_API void Initialize();
 *  - 接口类，即只有纯虚函数的类。
 *  - 内联类。所有函数要内联，且不存在类的静态成员变量和函数的静态变量。
 *  - 接口类和内联类的混合体。
 *  .
 * 上述规则如果不能遵守可能会导致链接错误或程序的崩溃。
 * @par 向导：
 * 制作一个新的类的时候，一般写一个接口类，然后派生一个实现类，实现虚函数。
 * 定义一个工厂函数，工厂函数所做的是创建实现类的对象（一般使用ONEU_NEW），并以接口类的指针返回。
 * 只有接口类和工厂函数暴露给外部程序，而新类隐藏起来。
 * 
 * @section section_principles 代码编写原则和风格
 *  - 命名采用Java代码风格。单词开头大写，单词之间无字符连接。成员函数第一个单词小写。
 *  - 类和结构体声明两个括号各占一行。函数声明第一个括号与函数头保持在同一行。
 *  - 工厂函数命名Klass_create。从工厂函数创建单件（或者其他）Klass_build，销毁单件Klass_destroy。
 *  - 名字前面有双下划线的为非常底层的符号，严禁客户调用。
 *  - 名字前有单下划线的为较底层的符号，使用起来需要谨慎但会比较高效。
 *  - 文件路径使用正斜杠。
 *  - 使用dword代替unsigned long，uint代替unsigned int。
 *  - 接口等头文件里的声明用Doxygen注释。
 *  - 禁止接口和内联函数、内联类中出现STL中的类。使用OneU的List和String代替。
 *  - 关于头文件哨兵。用\#pragma once。尽管有的编译器可能会不支持，但这样编译速度会快一点。
 *  - 不使用C++异常处理——不让异常处理跨越dll。严重异常可使用ONEU_RAISE结束程序。
 *  - operator new和operator delete重载需谨慎。
 *  - 谨慎使用全局变量，原则上只有Allocator单件被创建了以后其他类的实例才能够被创建，因此不允许任意定义全局变量。
 *  .
 * @section section_memory 内存管理策略
 *  - 派生自AllocatedObject的对象可以被直接管理内存（使用类属new等）。可以使用ONEU_NEW代替NEW来实现追踪内存（否则无法得知其语句的文件和行数等）。
 *  - 原生类型可以使用ONEU_NEW_T（以及ONEU_NEW_ARRAY_T），会使用默认内存池管理内存（单继承对象实质也能使用，不过不建议，最好派生自AllocatedObject）。
 *  - C++默认new依然有效，采用new处理原生类型或者普通对象依然是默认行为。
 *  .
 */

/* ----------------------------------------------------------------------------*/
/**
 * @page page_run 启动说明
 * 启动顺序
 *  - 进入主函数
 *	- 调用Game_build
 *		- 创建内存池系统
 *   	- 创建日志系统
 *   	- new出Game
 *			- 创建广播
 *			- 创建Atom系统（Atom本是建立在各系统模块之上的，建立在之前算作一个例外）
 *			- 创建图形系统
 *			- 创建控制系统
 *			- 创建场景
 *			.
 *  - 调用Game的init，建立顺序：图形系统，音响系统，最后是游戏逻辑。
 *  .
 *  销毁顺序（在IGame::run中）
 *   - 销毁场景
 *   - 广播销毁事件
 *   - 销毁Atom
 *   - 销毁控制系统
 *   - 销毁图形系统
 *   - 销毁广播系统
 *   .
 *  @remarks 一切单件都可以看做是在Game_build之后产生的，所以不能在Game_build之前使用内存池等单件。
 */
/* ----------------------------------------------------------------------------*/
/**
 * @page page_memory 内存管理
 * 使用自己的独有内存分配器管理内存。\n
 * 在当前的版本中，只有一个内存分配器，通过调用@ref GetAllocator来获取引用。\n
 * @ref AllocatedObject 是内存被管理的类的基类。派生自该类的对象使用new来分配内存时，自动会使用内存分配器。\n
 * 对于内存分配器的内存，可不受dll局限，在任何位置被创建和销毁。\n
 * C++默认的new仍然可以使用，但是按照C++的规则，同一个dll创建的内存必须在同一个dll里释放。\n
 * 为了使用方便，提供了一组宏用于分配内存。这组宏可以开启内存追踪功能，侦查内存泄露。\n
 * - ONEU_NEW 用于AllocatedObject的派生类。
 * - ONEU_NEW_T 用于原生类型。
 * - ONEU_NEW_ARRAY_T 用于原生类型数组。
 * - ONEU_ALLOC 直接分配内存。
 * .
 * 
*/
/* ----------------------------------------------------------------------------*/
/**
 * @page page_matrix_stack 矩阵栈(add 11.5.31)
 * @ref IVideo 包含矩阵栈操作函数，可以将矩阵压入栈中或者弹栈，此行为应在Video的绘制期间执行，即在某个INode的paint函数里。\n
 * 调用@ref IVideo::_getTransform 可获得当前的矩阵栈中所有矩阵相乘的结果，即当前INode绘制时所做的变换。其中调用@ref IVideo::renderImage 会自动应用矩阵栈的变换。
 * 其他情况若想支持矩阵栈的变化则要调用该函数获取变换并尝试应用到自己的绘图中。\n
 * \n
 * 每一个INode可选带有2D变换信息。若一个INode带有变换信息，则会在绘制时压入栈中。（此类似于OGRE中DeriveOrientation等……OGRE将其分解的更细小。此处看矩阵栈是存在缺陷的，
 * 关于矩阵栈暂时这样设计。）
 */
/* ----------------------------------------------------------------------------*/
/**
 * @page page_extending 扩展引擎
 * 扩展是这个引擎的核心。\n
 * 为引擎编写扩展可增加新功能，开发想要的效果。\n
 * 开发者可以获取底层的DX接口实现该引擎尚未实现的功能，也可以动用开源库为其增添一分力量。\n
 * @section section_extend_form_class 形式类别
 * @par 编写dll库——SDK式的扩展
 * 按照dll编写原则编写dll，发布dll和头文件，即可创建任何人可用的dll扩展。个人可以通过编译链接的方式使用该扩展。参见@ref section_interface_principles。
 * @par 编写at库——二进制动态语言符号扩展
 * 同样编写dll，按照Atom库编写标准，输出扩展名改为at即可。调用loadAtom即可加载该扩展，扩展会向Atom表中导出符号供动态语言使用。
 * 例子Waterfall_atom.cpp。
 * @par 编写脚本扩展
 * 与at库功用相同，只是扩展是使用动态语言编写的。
 * 暂未实现
 * @par 实现LUA脚本子系统
 * 实现需要读入外部数据的构件时，可以利用lua强大的灵活性，开发出次语言，用于描述数据、元数据等。
 * @section section_extend_functional_class 功能类别
 * @par 功能增强插件
 * 为引擎提供新功能（例如新的渲染特效）的插件。在软件工程里属于代码复用。
 * @par 框架插件
 * 为引擎提供逻辑框架的插件。在软件工程里属于设计复用。
 * 暂难以实现
 * @section section_atom_standards at扩展编写标准 reserved
 * @par 可选导出函数：void AtomInit(pcwstr)
 * 在加载的时候被调用，传入当前Atom版本号。
 * @par 可选导出函数：void AtomDestroy()
 * 在卸载的时候被调用。
 */
/* ----------------------------------------------------------------------------*/
/**
 * @page page_lua 使用LUA脚本编写游戏
 * 如何使用bin/game/下的执行包来制作游戏。\n
 * 该执行包是lua的执行版。由于Swig对lua的支持不完全。因此该版本在功能上也是不完全的，该版本不支持多场景和监听器。\n
 * 进入script/文件夹，编辑startup.lua文件。在这里配置游戏启动的内容。默认如下：
 * @code
 * require "OUE"
 * OUE.GetGame():init("Hello world", 800, 600, true);
 * @endcode
 * 进入script/文件夹，编辑main.lua文件。引擎会在游戏启动后会加载该文件。\n
 * 在文件中定义mainloop函数，每帧都会调用该函数；并在文件头包含OUE包：
 * @code
 * require "OUE"
 * function mainloop()
 * end
 * @endcode
 * 加载OUE库。
 * @section section_lua_display_image 显示图像
 * 拷贝picture.png图像到根目录。
 * 在mainloop函数前加入如下代码。
 * @code
 * sp = OUE.Sprite("picture.png");
 * OUE.addToScene(sp);
 * @endcode
 * 运行程序，有没有看到picture.png显示出来了？\n(addToScene(sp)等价于GetScene():getRenderScene():addChild(sp))
 * Sprite是标准库的符号，用于显示图片。
 * 这一句的含义就是建立新精灵，并将精灵对象作为场景的sp成员。
 * 建立出的精灵会显示在屏幕上。
 * @section section_lua_retrieve_control 响应控制
 * 关于响应控制有一种简单方案，就是查看输入设备的状态，并对精灵做出改变。
 * @code
 * if OUE.GetControl():keyIsDown(OUE.IControl_IK_RIGHT) then
 *	sp:setX(self.sp:getX() + 10.0f);
 * end
 * @endcode
 * 该句含义是，当发现方向键的右键被按下时，将精灵平移10像素。
 * @remarks 具体接口参见swig/*.i文件。
 */
/* ----------------------------------------------------------------------------*/
//注：测试代码 LuaInterpreter.cpp中将cpath设定在debug文件夹下，属测试代码。而release版由于没有把cpath设在release文件夹下，直接运行其实是不能跑的。
//注：AtomDemo工程目前处于不可用状态。

//@todo 制作Timer系统 延时一定时间执行某一函数
//@todo 更改Resource模块里的Release原型 使其返回引用数字
//@todo 将Resource并入主干部分
//@todo 支持RT，支持锁定纹理。
}
