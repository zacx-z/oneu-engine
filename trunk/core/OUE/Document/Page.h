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
 *  - @subpage page_script
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
 *	int ret = game->run();
 *  Game_destroy();
 *  return ret;
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
 * @par 为dll提供脚本支持
 * 使用SWIG，编写.i接口文件。必要时要包含 OUE\\swig 文件夹下的接口文件。
 */
/* ----------------------------------------------------------------------------*/
/**
 * @page page_script 使用脚本编写游戏
 * 目前脚本处于比较纠结的状态。
 * 目前实现了Lua和Ruby的接口，由于在SWIG里Lua不支持Directors，所以功能很受限，一般只能做单场景游戏。
 * 1.使用ruby(recommanded)
 * 提供便利函数@ref RubyRun。直接在主函数调用即可，不需要做任何其他事。
 * 除了Game_build, Game_destroy所有逻辑都在Ruby中编写。
 * 2.使用Lua
 * 需要自己编写一个场景（是唯一的场景），参考r19的Game\\main.cpp。(注：由于新版的改动，需要在run函数后调用Game_destroy）
 * Game_build，Game.run等基本逻辑在C++里编写。
 */
/**
 * @page page_problems PROBLEMS
 * PROBLEMS:
 * - Ruby和Lua使用方式有点迥异。参见上面。
 * - Ruby的异常处理与C是脱节的（貌似是使用longjmp)。也就是说如果使用Director时某些函数的局部变量会被直接抛弃。一些处理也会有点失控。
 *   对于这个问题，只要保证所有的系统资源间接被Game持有，Game_destroy确保被执行，就可以保证不出现资源泄露。
 * - 要先销毁脚本解释器再销毁Game，因为脚本解释器里的变量可能关乎资源。
 * - 相当多函数和类（主要是接口类，也有core中的类）都需要在有内存分配器的存在才可被使用。而Game_destroy会销毁掉内存分配器。所以在Game_build之前和Game_destroy之后都不能使用。
 *   对于上述Ruby逻辑编写方式而言，由于开始解释的时候没有内存分配器，因此连RubyInterpreter都不能创建。所以才会出现了不使用内存分配器的RubyRun便利函数。
 * - 关于脚本语言协作，涉及到Ownership，见SWIG。有时Script调用某函数会获取到Ownership，有时又会失去。当Ownership获取的时候一般通过返回值，而对于某些会失去Ownership时此时该变量必须有Ownership。
 * - win32：一旦调用了PostQuitMessage，就无法显示MessageBox了。
 * - 目前基本类型的typemap可能不够完善。
 * - 由于该SDK并不是完全面向脚本编写的，因此导出到脚本时出现了Ownership等复杂的问题（且由于SWIG是外部Wrap的方式，不具有内建的优势，如果面向脚本编写则要抛弃SWIG或自己写SWIG扩展）。
 *	- 每次调用Get函数或其他方法获取某个对象时，其实是新建了一个script里的Wrapper，所以一个对象可能对应许多wrapper。而最多一个有ownership，只有有ownership的对象才能销毁内容。而如果通过调用某个函数ownership被C++内核拿走，则要求传入的必须是有ownership的对象。这为脚本带来了不必要的复杂性。如果想消除这种复杂性，则要在每个导出的类里定义一个对应脚本里的VALUE，也就是侵入式编程，一要框架重新设计，二则难以使用SWIG。（SWIG是非侵入式编程）
 *	- 如果使用SWIG的默认库的话，不满足能查看一个变量是否有ownership的要求。因此自己写了一段代码，这段代码与SWIG对应语言的具体实现强相关，如果出现大改动可能失效。所以暂时推荐swig2.0.4。
 *  - SWIG导出返回pcwstr的director虚函数可能会有内存被提前释放的问题。（未验证）
 *	.
 * .
 * 脚本与C++边缘（容易出莫名其妙错误的地方，需要特别注意）：
 * - @ref IGame::pushInputReceiver，如果不在脚本中保持该变量，则会引起空引用错误，且该错误出现的时机是随机的。
 * - @ref IGame::replaceScene，如果不保持返回的变量，会自动销毁原来的场景，一般不建议使用返回的变量，会丢失掉脚本部分的信息，会很麻烦。
 * - @ref INode::addChild不将ownership转移给父节点，脚本若不保持该变量可能会使显示元素消失。
 * .
 * @attention 重要改动，Game.run后需要调用Game.destroy
 * @remarks 具体接口参见swig/*.i文件。
 */
/* ----------------------------------------------------------------------------*/
//注：测试代码 LuaInterpreter.cpp中和RubyInterpreter.cpp将cpath设定在debug文件夹下，属测试代码。而release版由于没有把cpath设在release文件夹下，直接运行其实是不能跑的。
//注：AtomDemo工程目前处于不可用状态。

//@todo 制作Timer系统 延时一定时间执行某一函数
//@todo 更改Resource模块里的Release原型 使其返回引用数字
//@todo 将Resource并入主干部分
//@todo 支持RT，支持锁定纹理。
}
