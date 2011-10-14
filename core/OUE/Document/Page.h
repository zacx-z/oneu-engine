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
 *  - 音响接口 @ref IAudio\n
 *  - 图形接口 @ref IVideo\n
 *  负责显示和管理图像、动画。
 *  - 控制接口 @ref IControl\n
 *  负责处理键盘和鼠标等输入设备。
 *  - 场景接口 @ref IScene\n
 *  负责处理游戏逻辑。一个场景是一个逻辑单元，被游戏对象所持有和处理。
 * .
 * 
 * @subsection section_event 事件系统(未完成，depracated)
 * 事件系统由事件派发器，监听器和事件（包括事件参数）组成。使用字符串来区分一类事件。事件派发器是接受事件的类，通过向事件派发器添加监听器来处理某一个事件。监听器可添加多个。\n
 * 目前的事件机制没有采用队列，是在发送消息的同时执行监听器的触发函数。\n
 * 公共事件会通过广播系统传播。广播类是事件派发器的继承类。\n
 * 键盘事件：调用@ref IScene::pushInputFocus将一个事件派发器设为输入焦点，该事件派发器可以接收到键盘的输入事件。
 * @subsection section_renderscene 图形系统和渲染场景
 * 万有引擎使用树来渲染。树中的每一个节点即表示一个渲染操作。渲染时会对树进行遍历。\n
 * 一个渲染场景即为一个渲染树。一般而言，一个场景对应一个渲染场景。\n
 * @ref video::INode即是渲染节点的接口，@ref video::ILayer是允许有孩子的渲染节点接口。z值表示一个渲染节点在其兄弟节点被渲染的顺序，z值小的会先被渲染。\n
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
 *	Game_build();
 *	IGame* game = GetGame();
 *	Aux_GameInit(L"Hello World", 800, 600, true);
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
 * @subpage page_memory。
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
 * @ref page_memory
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
 * 重写IAllocator可以实现自定义的内存分配器，用于原生类型。AllocatedObject目前不支持自定义内存分配器。\n
 * C++默认的new仍然可以使用，但是按照C++的规则，同一个dll创建的内存必须在同一个dll里释放。\n
 * 为了使用方便，提供了一组宏用于分配内存。这组宏可以开启内存追踪功能，侦查内存泄露。\n
 * - ONEU_NEW 用于AllocatedObject的派生类。
 * - ONEU_NEW_T 用于原生类型。
 * - ONEU_NEW_TA 用于原生类型，使用自定义内存分配器。
 * - ONEU_NEW_ARRAY_T 用于原生类型数组。
 * - ONEU_NEW_ARRAY_TA 用于原生类型数组，使用自定义内存分配器。
 * - ONEU_ALLOC 直接分配内存。
 * .
 * 
*/
/* ----------------------------------------------------------------------------*/
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
 * 使用ruby编写游戏。\n
 * C++部分：参见Game/main.cpp，在主函数调用便利函数@ref RubyRun。\n
 * Ruby脚本：程序所在文件夹下编写script/main.rb。\n
 * 除了Game_build, Game_destroy所有逻辑都在Ruby中编写。\n
 * 请checkout OUE/swig下代码来获取脚本参考。
 */
/* ----------------------------------------------------------------------------*/
/**
 * @page page_problems PROBLEMS
 * PROBLEMS:
 * - Ruby的异常处理与C是脱节的（貌似是使用longjmp)。也就是说如果使用Director时某些函数的局部变量会被直接抛弃。一些处理也会有点失控。
 *   对于这个问题，只要保证所有的系统资源间接被Game持有，Game_destroy确保被执行，就可以保证不出现资源泄露。
 * - 要先销毁脚本解释器再销毁Game，因为脚本解释器里的变量可能关乎资源。
 *   对于上述Ruby逻辑编写方式而言，由于开始解释的时候没有内存分配器，因此连RubyInterpreter都不能创建。所以才会出现了不使用内存分配器的RubyRun便利函数。
 * - 关于脚本语言协作，涉及到Ownership，见SWIG。有时Script调用某函数会获取到Ownership，有时又会失去。当Ownership获取的时候一般通过返回值，而对于某些会失去Ownership时此时该变量必须有Ownership。Ownership的转移会带来过多复杂性，已经被取消。
 * - win32：一旦调用了PostQuitMessage，就无法显示MessageBox了。
 * - 目前基本类型的typemap可能不够完善。
 * - 由于该SDK并不是完全面向脚本编写的，因此导出到脚本时出现了Ownership等复杂的问题（且由于SWIG是外部Wrap的方式，不具有内建的优势，如果面向脚本编写则要抛弃SWIG或自己写SWIG扩展）。
 *		- 每次调用Get函数或其他方法获取某个对象时，其实是新建了一个script里的Wrapper，所以一个对象可能对应许多wrapper。而最多一个有ownership，只有有ownership的对象才能销毁内容。而如果通过调用某个函数ownership被C++内核拿走，则要求传入的必须是有ownership的对象。这为脚本带来了不必要的复杂性。如果想消除这种复杂性，则要在每个导出的类里定义一个对应脚本里的VALUE，也就是侵入式编程，一要框架重新设计，二则难以使用SWIG。（SWIG是非侵入式编程）
 *		- SWIG导出返回pcwstr的director虚函数可能会有内存被提前释放的问题。（未验证）
 *		.
 * .
 * 脚本与C++边缘（容易出莫名其妙错误的地方，需要特别注意）：
 * - @ref IScene::pushInputFocus，如果不在脚本中保持该变量，则会引起空引用错误，且该错误出现的时机是随机的。
 * - @ref IGame::replaceScene，同上，需要使用变量保持（建议用全局变量），否则会引起段错误。
 * - @ref video::ILayer::addChild不将ownership转移给父节点，脚本若不保持该变量可能会使显示元素消失。
 * .
 * @remarks 具体接口参见swig/*.i文件。
 */

//注：测试代码 RubyUtil.cpp将cpath设定在debug文件夹下，属测试代码。而release版由于没有把cpath设在release文件夹下，直接运行其实是不能跑的。

/**
 * @page page_ruby_reference ruby参考
 *
 * <h2>Basic Types</h2>
 * 注：{xyz}表示该处字符可用大括号中任意字符（如x，y，z）替换，是从openGL文档中借鉴过来的写法。（一般i表示int，f表示浮点数）
 * <h3>Color</h3>
 * 三个或四个浮点数的数组表示为Color类型，分别代表颜色的red, green, blue, alpha分量。
 * <h3>Rect{if}.new</h3>
 * <h3>Rect{if}.new(left, top, right, bottom)</h3>
 * 用四个坐标表示一个矩形。
 * 属性：
 *  - left
 *  - top
 *  - right
 *  - bottom
 *  .
 *
 * <h3>Vector{234}{if}.new</h3>
 * <h3>Vector{234}{if}.new(x, y, [z, [w]])</h3>
 * 2~4维向量。
 * 属性：
 *  - x
 *  - y
 *  - z（3维向量，4维向量）
 *  - w（4维向量）
 *  .
 *
 * <h2>Module Functions</h2>
 *
 * <h3>Aux_GameInit(windowTitle, width, height, windowed)</h3>
 * 辅助库：初始化函数。初始化各系统。未给出详细配置系统的能力。
 * 初始化windowTitle名称的窗口，以width*height为分辨率。若windowed为false，则游戏是全屏的。
 * <h3>GetGame</h3>
 * 获取主游戏。Game类的实例。
 * <h3>GetVideo</h3>
 * 获取图形模块。Video类的实例。
 * <h3>GetAudio</h3>
 * 获取音响模块。Audio类的实例。
 * <h3>GetControl</h3>
 * 获取控制模块。Control类的实例。
 * <h3>GetScene</h3>
 * 获取场景。Scene类的实例。
 *
 *
 * <h2>Game Class</h2>
 * 管理所有游戏资源的顶层类。
 *
 * <h3>getVideo</h3>
 * 获取图形模块。Video类的实例。
 * <h3>getAudio</h3>
 * 获取音响模块。Audio类的实例。
 * <h3>getControl</h3>
 * 获取控制模块。Control类的实例。
 * <h3>getScene</h3>
 * 获取场景。Scene类的实例。
 * 
 * <h3>init(windowTitle, width, height, windowed)</h3>
 * 初始化。注意该函数并不初始化各模块。一般使用Aux_GameInit初始化。
 * <h3>run</h3>
 * 启动游戏。
 * <h3>quit</h3>
 * 退出游戏。
 *
 * <h3>onFrame</h3>
 * 为游戏发送帧事件。
 * <h3>onActiveWindow(actived)</h3>
 * 为游戏窗口发送激活或反激活事件。
 *
 * <h3>setWindowTitle</h3>
 * 设置窗口标题。
 * <h3>getWindowPos</h3>
 * 获取窗口位置。
 * <h3>setWindowPos(pos)</h3>
 * 设置窗口位置。
 * 窗口位置用Vector2i表示。
 * <h3>showCursor(show)</h3>
 * 设置光标显示状态。
 *
 * <h3>getFPS</h3>
 * 获取当前FPS。
 * <h3>getTimeInverval</h3>
 * 获取这一帧的长度。
 * <h3>replaceScene(scene)</h3>
 * 替换新场景。
 * @attention 需要保留scene的引用，否则scene可能会因为垃圾回收而消失进而抛出错误。
 * 
 * <h3>onChar</h3>
 * 发送字符事件。
 * <h3>onKey</h3>
 * 发送按键事件。
 * <h3>onMouse</h3>
 * 发送鼠标事件。
 *
 * <h3>runShell</h3>
 * <h3>output</h3>
 *
 * <h3>showInfo</h3>
 *
 * 
 * <h2>Video Class</h2>
 * 图形模块。
 * <h3>Constant(in Module)</h3>
 *  - Blend Mode
 *    混合模式。决定了该图形以何种方式作用到画布上。
 *  	- BM_NORMAL
 *  	普通模式。
 *  	- BM_ADD
 *  	加法模式。
 *  	.
 *  - Color Blend Mode
 *    颜色混合模式。将该图形与某种颜色以该模式混合画到画布上。可将表方位的和算法的结合使用。
 *  	- CBM_NONE
 *  	不混合。
 *  	- CBM_ADD
 *  	加法模式。
 *  	- CBM_MODULATE
 *  	调制模式。即乘法模式。
 *  	- CBM_LEFT
 *  	左混合。
 *  	- CBM_RIGHT
 *  	右混合。
 *  	- CBM_TOP
 *  	上混合。
 *  	- CBM_DOWN
 *  	下混合。
 *  	.
 *  .
 *
 * <h3>Image_t Class</h3>
 * 图像类。管理一块图像缓存。
 * 只读属性：
 *  - width
 *  - height
 *  .
 *
 *  
 * <h3>getName</h3>
 * <h3>prepare</h3>
 * <h3>init</h3>
 * 
 * <h3>switchDevice(width, height, windowed)</h3>
 * 切换设备，到width * height分辨率。若windowed为true，则窗口化。
 * <h3>isWindowed</h3>
 * 是否窗口化
 * <h3>getDeviceSize</h3>
 * 获取分辨率大小。
 * <h3>loadImage(filename)</h3>
 * 读取图像。
 * <h3>getRoot</h3>
 * 获取根渲染结点。
 * <h3>getRenderScene</h3>
 * 获取当前渲染场景。
 * <h3>setRenderScene</h3>
 * 设置当前渲染场景。
 * <h3>createRenderScene</h3>
 * 创建渲染场景。
 * <h3>showInfo</h3>
 * <h3>getAvailableMode</h3>
 * 获取允许运行的显示模式。
 *
 * 
 * <h2>Audio Class</h2>
 * 音响模块。
 *
 * <h3>Sound_t</h3>
 *
 * <h3>init</h3>
 * <h3>loadSound(filename, streamed)</h3>
 * 读取声音。目前只支持ogg。当streamed为true，声音以流载入。
 * <h3>playMusic</h3>
 * 将声音作为背景音乐播放。
 * <h3>stopMusic</h3>
 * 停止当前背景音乐。
 * <h3>getPlayingMusic</h3>
 * 获取当前声音。
 * <h3>playFX</h3>
 * 播放特效。
 *
 *
 * <h2>Control Class</h2>
 * 
 * <h3>init</h3>
 * <h3>keyIsDown(scancode)</h3>
 * <h3>keyPress(scancode)</h3>
 * <h3>keyRelease(scancode)</h3>
 * 分别表示扫描码为scancode的键盘按键是否被按下、在该帧被按下、在该帧被释放。
 * <h3>buttonIsDown(id)</h3>
 * <h3>buttonRelease(id)</h3>
 * <h3>buttonPress(id)</h3>
 * 分别表示id代表的鼠标按键是否被按下、在该帧被按下、在该帧被释放。
 * <h3>mouseOffset</h3>
 * 前一帧到当前帧间隔内鼠标移动的偏移量。
 *
 * <h3>Key Scan Code Constant(in Module)</h3>
 * @ref page_key_scan_code
 *
 * <h2>Scene Class</h2>
 * Abstract Class.
 *
 * <h3>RS</h3>
 * 该场景对应的渲染场景。inherits Layer.
 * <h3>pushInputFocus(ir)</h3>
 * <h3>popInputFocus</h3>
 * 输入焦点压栈，弹栈。弹栈返回顶端的输入焦点。
 * 
 * override
 * <h3>update(dt)</h3>
 * 更新函数。dt为当前帧的时间长度。需要被重写。
 *
 *
 * <h2>VideoNode Class</h2>
 *
 * 可渲染节点类。
 * <h3>{xy}</h3>
 * <h3>{xy}=</h3>
 * x，y坐标。
 * <h3>o{xy}</h3>
 * <h3>o{xy}=</h3>
 * 中心x，y坐标。
 * <h3>rotation</h3>
 * <h3>rotation=</h3>
 * 旋转度。逆时针为正。以角度计。
 * <h3>scale{XY}</h3>
 * <h3>scale{XY}=</h3>
 * x，y上的缩放。
 * <h3>visible</h3>
 * <h3>visible=</h3>
 * 是否可见。
 * <h3>active</h3>
 * <h3>active=</h3>
 * 是否活动。（即是否会调用更新函数）
 *
 * <h3>parent</h3>
 * 父节点。
 * <h3>detach</h3>
 * 从父节点中脱离。
 * <h3>update(dt)</h3>
 * 更新节点。
 * <h3>name</h3>
 * <h3>create2DTransform</h3>
 * 创建2D变换，某些渲染节点要创建2D变换才能使变换属性有作用。//todo
 *
 * <h2>Layer Class</h2>
 * Inherits VideoNode.
 * <h3>name</h3>
 * <h3>addChild(child, z)</h3>
 * 加入孩子。z为所在深度。
 * @attention 需要在脚本中保留child的引用，否则该节点可能会因为垃圾回收而消失。
 *
 *
 * <h2>Sprite Class</h2>
 * Inherits VideoNode.
 * 精灵，显示图像。
 *
 * <h3>new(filename)</h3>
 * <h3>new(image)</h3>
 * 创建精灵。
 * <h3>o{hv}</h3>
 * <h3>o{hv}=</h3>
 * 中心坐标，归一化后。
 * <h3>color</h3>
 * <h3>color=</h3>
 * 叠加的颜色。
 * <h3>alpha</h3>
 * <h3>alpha=</h3>
 * 透明度。
 * <h3>srcRect</h3>
 * <h3>srcRect=</h3>
 * 从图形文件获取的源矩形。
 * <h3>width</h3>
 * <h3>height</h3>
 * 图形宽高。
 *
 * <h3>image</h3>
 * <h3>image=</h3>
 * 显示图像。
 *
 * <h3>blendMode</h3>
 * <h3>blendMode=</h3>
 * 混合模式。
 * <h3>colorBlendMode</h3>
 * <h3>colorBlendMode=</h3>
 * 颜色混合模式。
 * 
 * <h2>Label Class</h2>
 * Inherits VideoNode.
 * 标签，显示文本。
 *
 * <h3>Constant(in Module)</h3>
 * 对齐方式
 *  - T_LEFT
 *  - T_RIGHT
 *  - T_CENTER
 *  - T_TOP
 *  - T_BOTTOM
 *  - T_VCENTER
 *  .
 * 
 * <h3>new(width, height, fontSize, font = default)</h3>
 * 创建标签。font为字符串，表示字体名称。
 * <h3>alpha</h3>
 * <h3>alpha=</h3>
 * 透明度。
 * <h3>text</h3>
 * <h3>text=</h3>
 * 显示文本。
 * <h3>color</h3>
 * <h3>color=</h3>
 * 文本颜色。
 * <h3>align</h3>
 * <h3>align=</h3>
 * 文本对齐方式。
 * 
 *
 * <h2>Shape Class</h2>
 * Inherits VideoNode.
 * 形状。
 * 
 * <h3>new(rect)</h3>
 * 创建矩形形状。
 * <h3>color</h3>
 * <h3>color=</h3>
 * 形状颜色。
 * <h3>mode</h3>
 * <h3>mode=</h3>
 * 是否只绘制边框。
 *
 * <h2>Event Classes</h2>
 * <h3>CharEvent</h3>
 * getChar - 返回字符。
 * <h3>KeyEvent</h3>
 * getCode - 返回扫描码。
 * isRelease - 是否为按键释放事件。
 * isPress - 是否为按键按下事件。
 * <h3>MouseEvent</h3>
 * getButtonID - 获得按钮ID。
 * isRelease - 是否为按钮释放事件。
 * isPress - 是否为按钮按下事件。
 *
 *
 * <h2>InputReceiver Classes</h2>
 * 输入接收器。
 * 可用来重写来定制行为。
 * override
 * <h3>onChar(event)</h3>
 * 接收CharEvent，当活动InputReceiver收到字符事件时被调用。
 * <h3>onKey(event)</h3>
 * 接收KeyEvent，当活动InputReceiver收到键盘事件时被调用。
 * <h3>onMouse(event)</h3>
 * 接收MouseEvent，当活动InputReceiver收到鼠标事件时被调用。
 *
 * ...todo remove BroadCast?
 * ...todo remove Shell?
 */
/**
 * @page page_key_scan_code 扫描码列表
 *
 * - IK_ESCAPE
 * - IK_1
 * - IK_2
 * - IK_3
 * - IK_4
 * - IK_5
 * - IK_6
 * - IK_7
 * - IK_8
 * - IK_9
 * - IK_0
 * - IK_MINUS
 * - IK_EQUALS
 * - IK_BACK
 * - IK_TAB
 * - IK_Q
 * - IK_W
 * - IK_E
 * - IK_R
 * - IK_T
 * - IK_Y
 * - IK_U
 * - IK_I
 * - IK_O
 * - IK_P
 * - IK_LBRACKET
 * - IK_RBRACKET
 * - IK_RETURN
 * - IK_LCONTROL
 * - IK_A
 * - IK_S
 * - IK_D
 * - IK_F
 * - IK_G
 * - IK_H
 * - IK_J
 * - IK_K
 * - IK_L
 * - IK_SEMICOLON
 * - IK_APOSTROPHE
 * - IK_GRAVE
 * - IK_LSHIFT
 * - IK_BACKSLASH
 * - IK_Z
 * - IK_X
 * - IK_C
 * - IK_V
 * - IK_B
 * - IK_N
 * - IK_M
 * - IK_COMMA
 * - IK_PERIOD
 * - IK_SLASH
 * - IK_RSHIFT
 * - IK_MULTIPLY
 * - IK_LMENU
 * - IK_SPACE
 * - IK_CAPITAL
 * - IK_F1
 * - IK_F2
 * - IK_F3
 * - IK_F4
 * - IK_F5
 * - IK_F6
 * - IK_F7
 * - IK_F8
 * - IK_F9
 * - IK_F10
 * - IK_NUMLOCK
 * - IK_SCROLL
 * - IK_NUMPAD7
 * - IK_NUMPAD8
 * - IK_NUMPAD9
 * - IK_SUBTRACT
 * - IK_NUMPAD4
 * - IK_NUMPAD5
 * - IK_NUMPAD6
 * - IK_ADD
 * - IK_NUMPAD1
 * - IK_NUMPAD2
 * - IK_NUMPAD3
 * - IK_NUMPAD0
 * - IK_DECIMAL
 * - IK_OEM_102
 * - IK_F11
 * - IK_F12
 * - IK_F13
 * - IK_F14
 * - IK_F15
 * - IK_KANA
 * - IK_ABNT_C1
 * - IK_CONVERT
 * - IK_NOCONVERT
 * - IK_YEN
 * - IK_ABNT_C2
 * - IK_NUMPADEQUALS
 * - IK_PREVTRACK
 * - IK_AT
 * - IK_COLON
 * - IK_UNDERLINE
 * - IK_KANJI
 * - IK_STOP
 * - IK_AX
 * - IK_UNLABELED
 * - IK_NEXTTRACK
 * - IK_NUMPADENTER
 * - IK_RCONTROL
 * - IK_MUTE
 * - IK_CALCULATOR
 * - IK_PLAYPAUSE
 * - IK_MEDIASTOP
 * - IK_VOLUMEDOWN
 * - IK_VOLUMEUP
 * - IK_WEBHOME
 * - IK_NUMPADCOMMA
 * - IK_DIVIDE
 * - IK_SYSRQ
 * - IK_RMENU
 * - IK_PAUSE
 * - IK_HOME
 * - IK_UP
 * - IK_PRIOR
 * - IK_LEFT
 * - IK_RIGHT
 * - IK_END
 * - IK_DOWN
 * - IK_NEXT
 * - IK_INSERT
 * - IK_DELETE
 * - IK_LWIN
 * - IK_RWIN
 * - IK_APPS
 * - IK_POWER
 * - IK_SLEEP
 * - IK_WAKE
 * - IK_WEBSEARCH
 * - IK_WEBFAVORITES
 * - IK_WEBREFRESH
 * - IK_WEBSTOP
 * - IK_WEBFORWARD
 * - IK_WEBBACK
 * - IK_MYCOMPUTER
 * - IK_MAIL
 * - IK_MEDIASELECT
 *
 * - IK_BACKSPACE
 * - IK_NUMPADSTAR
 * - IK_LALT
 * - IK_CAPSLOCK
 * - IK_NUMPADMINUS
 * - IK_NUMPADPLUS
 * - IK_NUMPADPERIOD
 * - IK_NUMPADSLASH
 * - IK_RALT
 * - IK_UPARROW
 * - IK_PGUP
 * - IK_LEFTARROW
 * - IK_RIGHTARROW
 * - IK_DOWNARROW
 * - IK_PGDN
 *
 * - IK_CIRCUMFLEX
 * .
 * 
 */
//@todo 支持RT，支持锁定纹理。
//@todo oload等详细写。
}
