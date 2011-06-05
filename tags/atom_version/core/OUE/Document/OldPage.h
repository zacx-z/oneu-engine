namespace OneU
{
	namespace DX
	{
		/*@cond OLD_PAGE*/
		/* ----------------------------------------------------------------------------*/
		/**
		* @page dxhelper DX封装层
		*
		*
		* 目前由图形和输入两个模块构成。
		* @section DXGraphics 图形模块 
		*
		* 用 #Graphics 全局变量操作图形设备。
		* @subsection UseGraphics 图形设备的使用
		*	-# 调用 Graphics_t::PreInit 让设备进行准备工作。
		*	-# 此时可调用 Graphics_t 的一系列成员函数来检查设备 主要是一些参数的支持情况
		*	-# 调用 Graphics_t::InitWindowed 或 Graphics_t::InitFullScreen 来初始化设备。设备进入工作阶段。可创建各种设备资源。
		*	-# 绘图等操作将图像绘制到后备缓冲（显存）中，需调用 Graphics_t::Present 将其显现。这样便为一帧。
		*	-# 当丢失设备（抛出 CDXException 异常，且异常码为DEVICELOST）时，应释放所有显卡资源。调用 Graphics_t::ResetWindowed 或 Graphics_t::ResetFullScreen 来完成重置设备。
		*	-# 程序结束时设备会自动销毁。但是设备资源需要注意，若没有正确销毁会造成资源泄漏。
		*	.
		* 
		* @subsection GraphicsResourceFeature 图形资源类特性
		*	- 构造函数是nothrow且不做任何设备动作的。
		*	- 在析构函数中都会自动释放资源。
		*	- 不可使用拷贝构造函数和赋值运算符。
		*	-# 纹理资源使用参见 @ref texturehelper
		*	.
		* @subsection DXGraphicsE 图形辅助库
		*	- 类库\n
		*		成员函数：OnLostDevice在丢失设备时调用。OnResetDevice在重设设备后调用。\n
		*		析构函数会释放资源。
		*		- FontE：对应ID3DXFont。用于绘制文字，在类中使用了设备资源。有预读功能，可以优化写文字的速度。使用时定义一个新的对象并调用FontE::Create()函数。
		*		- LineE：对应ID3DXLine。用于绘制线条。具有全局对象，可直接使用。也可定于新对象调用LineE::Create()函数。
		*		- SpriteE：对应ID3DXSprite。用于绘制二维图像，但不能使用着色器。具有全局对象，可直接使用。也可定于新对象调用SpriteE::Create()函数。
		*		.
		*	- 函数库
		*		- MatrixE：封装对矩阵的常用运算。
		*		- ImageE：读取图像文件，装载到纹理里。
		*		.
		*	.
		*
		* 详细内容：
		* - @subpage texturehelper
		* - @subpage vertexhelper
		* - @subpage rendering
		* .
		* @section DXInput 输入模块
		* 用 #Input 全局变量操作输入设备。
		* -# 调用Input_cls::Init初始化输入系统。
		* -# 定义输入设备对象，输入设备基类有统一基类InputDevice，非多态的。预置派生类型有：
		*		- KeyBoard 键盘设备
		*		- Mouse 鼠标设备
		*		.
		* -# 调用Input_cls::Create创建设备，参数表示缓冲区大小。若设为0则没有缓冲区，设备就不支持缓冲模式。
		* -# 调用Input_cls::Acquire获得设备。
		* -# 调用派生类的Read函数通过立即模式读取设备数据。
		* -# 调用HandleData函数通过缓冲模式读取设备数据。
		* .
		*/
		/* ----------------------------------------------------------------------------*/
		/* ----------------------------------------------------------------------------*/
		/**
		* @page texturehelper 纹理资源
		* 
		* @section Surface 表面
		* 表面即内存或显存中连续的一段所存储的图像。
		* 表面上的信息有长宽，以及像素格式。
		* 使用DX上的表面 Surface_t 必须指定其使用D3D内存池类型作为模板参数。所谓内存池即为它的存储方式。可参考Direct3D手册D3DPOOL。\n
		* 为了能够取得OpenGL和DX的统一性，建议使用typedefs而不是指定其D3D内存池。\n
		* 目前支持的typedefs：
		* - #Surface 在显存或内存中被Direct3D管理的表面，当丢失设备时要释放，不可被锁定。
		* - #Surface_Lk 在内存中的表面，可被锁定。
		* .
		* 表面是不可被渲染的。
		* @section Texture 纹理
		* 由于MipMap技术的关系，一个纹理可能含有多个表面，可看做表面的集合。
		* 纹理上的信息有长宽，以及像素格式。
		* 使用DX上的纹理 Texture_t 必须指定其实用D3D内存池类型以及使用法作为模板参数。所谓内存池即为它的存储方式，使用法告诉了如何让D3D管理的细节。可参考Direct3D手册D3DPOOL、D3DUSAGE。\n
		* 为了能够取得OpenGL和DX的统一性，建议使用typedefs而不是指定其D3D内存池和使用法。\n
		* 目前支持的typedefs：
		* - #Texture 在显存或内存中被Direct3D管理的表面。当丢失设备时要释放，不可被锁定。
		* - #Texture_Lk 在内存中的纹理，可被锁定。
		* - #Texture_RT 可作为渲染目标的纹理，在显存中。当丢失设备时要释放，不可被锁定。
		* .
		* @subsection TextureRendering 渲染纹理
		* 因为使用的是3D库，所以除了辅助库SpriteE，没有直接渲染纹理到后备缓冲的办法。
		* 想要渲染纹理必须用把纹理做贴图贴到网格的办法。
		* 方法是定义 VertexUP 顶点数组，大小为4，设定它们的位置和纹理 渲染顶点。详情参见@ref rendering
		* @remarks
		* MipMap技术是一种尽量保证纹理在缩放渲染时不失真且足够快速的技术，是一种空间换时间的思想。一个图像在内存或显存中存储为多个表面，第一个表面为图像本身的长宽，后一个表面长宽分别为前一个的二分之一。渲染时选择最接近大小的两个表面进行三线性过滤（也有可能是其他过滤方式，如各向异性过滤）。
		* DX中在创建纹理时的Level参数表示了这个纹理要存为几个表面。指定为0时，它会建立多个表面一直将图像缩放到1*1为止。
		* 渲染目标
		*/
		/* ----------------------------------------------------------------------------*/
		/* ----------------------------------------------------------------------------*/
		/**
		* @page vertexhelper 顶点处理
		*
		* 3D库中的渲染是脱离不了顶点的：每个面、每条边都是顶点构成的，而纹理必须贴到面上才能够被渲染。\n
		* DX支持两种使用顶点的方式。一种是顶点作为数组存储在程序中（存储在内存中），对应顶点较少的情况。一种是在显存或内存中开辟缓冲区存储顶点，对应顶点较多的情况。由于此库为2D库，只支持第一种情况。\n
		* VertexUP 可理解为存储单个顶点的结构体，使用时一般会声明一个数组。模板参数传入 FVF_FLAG 枚举值，用来表示它所含的成员，各种标志用“|”连接。在参数包含某成员的时候，可调用对应成员的函数来对其进行操作。\n
		* 例子：\n
		* @code
		* OneU::DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > v[4];
		* v[0].SetPos( 0.0f, 0.0f, 0.0f );v[0].Diffuse() = SetRGB( 256, 256, 256 );
		* v[1].SetPos( 0.0f, 1.0f, 0.0f );v[1].Diffuse() = SetRGB( 256, 256, 256 );
		* v[2].SetPos( 1.0f, 1.0f, 0.0f );v[2].Diffuse() = SetRGB( 256, 256, 256 );
		* v[3].SetPos( 1.0f, 0.0f, 0.0f );v[3].Diffuse() = SetRGB( 256, 256, 256 );
		* @endcode
		*
		* @section TransformedVertex 未变换的与变换后的顶点
		* #FVF_XYZRHW 与 #FVF_XYZ 是不兼容的（即不能同时作为某顶点的标志）。两个标志都包含X,Y,Z成员。前一个标志还包含RhW成员。\n
		* 第一个标志表示已变换后的顶点。X,Y表示像素的坐标，以左上角为(0,0)。Z坐标不用，因此也可理解为2D的顶点。RhW要赋值为1，否则会有不可预料的效果（没什么后果……）。
		* 第二个标志表示未变换后的顶点，会经过矩阵变换才会呈现到屏幕上。关于矩阵变换的详情参见@ref MatrixTransforming。
		*
		* @section UVMap 顶点的UV坐标
		*
		* 关于顶点的渲染参考@ref rendering。
		*/
		/* ----------------------------------------------------------------------------*/
		/* ----------------------------------------------------------------------------*/
		/**
		* @page rendering 渲染
		* 
		* 定义一个 RenderManip 对象表示渲染开始。该对象的整个生存期表示渲染过程，随着对象的销毁渲染过程也就结束。
		* @section vertexrendering 顶点渲染
		*
		* 渲染一个VertexUP数组表示的顶点：
		* -# 首先设定好FVF可变顶点格式。
		* -# 可选：如果顶点是未变换的顶点，可设定顶点变换矩阵。参见@ref TransformedVertex。
		* -# 可选：如果顶点包含纹理坐标信息，设定对应的TStage的纹理。
		* -# 可选：如果顶点包含多重纹理或Diffuse位，可设定对应的TStage颜色混合模式和Alhpa混合模式。
		* -# 调用 RenderManip::RenderVertexUP() 来渲染一组顶点。
		* .
		* @attention RenderManip::RenderVertexUP() 最后一个参数表示的是图元的数目，不是顶点的数目。该参数是与第一个参数有关的。例如四个顶点，如果图形类型是点列的话就为4，如果是三角形扇的话就为2。
		* @remarks 面的渲染还跟CullMode有关。CullMode会区分一个面的正面和背面，背面朝着玩家的面是不会被渲染的。默认情况下看过去面的顶点是逆时针存储的是正面。可用Graphics_t::SetCullMode() 来设定。
		*
		* 一个绘制矩形的例子：
		* @code
		* //顶点位置及颜色的设定
		* OneU::DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > v[4];
		* v[0].SetPos( 0.0f, 0.0f, 0.0f );v[0].Diffuse() = SetRGB( 256, 256, 256 );
		* v[1].SetPos( 0.0f, 1.0f, 0.0f );v[1].Diffuse() = SetRGB( 256, 256, 256 );
		* v[2].SetPos( 1.0f, 1.0f, 0.0f );v[2].Diffuse() = SetRGB( 256, 256, 256 );
		* v[3].SetPos( 1.0f, 0.0f, 0.0f );v[3].Diffuse() = SetRGB( 256, 256, 256 );
		* {//开始渲染
		*		OneU::DX::RenderManip rd;
		*		rd.RenderVertexUP( OneU::DX::Render::TRIANGLEFAN, v, 2 );
		* }//结束渲染
		* @endcode
		*
		* @section MatrixTransforming 矩阵变换
		* 
		* 矩阵变换时固定渲染管线必经的流程。顶点和纹理绘制前要经过世界变换、视窗变换、投影变换才能绘制到屏幕上。有不经过变换的顶点，参见@ref TransformedVertex。使用顶点着色器时由于不经过固定管线，所以不会经过固定管线的矩阵变换。
		*
		* @section TStageSet TStage（TextureStage）设定
		*
		* TextureStage能够控制纹理的绘制。
		*
		*/
		/* ----------------------------------------------------------------------------*/

		/*@endcond*///OLD_PAGE
}}