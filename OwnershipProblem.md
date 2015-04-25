# Ownership #

在C++程序中，一般内存使用遵循着谁创建谁销毁的原则，这样的策略能避免复杂的函数定义以及由此引出的内存泄露，另外也能避免dll堆损坏（因为在一个dll堆中new的内存在另一个dll中delete的话会引起堆损坏）。
但这样也带来了一定的麻烦，举个例子：
```
m_pListener = new Listener;
something.addListener(m_pListener);
//......
//somewhere else, such as in destructor
delete m_pListener;
```
很多时候我们更喜欢如下的写法：
```
something.addListener(new Listener);
```
这样，新建的对象就交由something在必要的时候销毁了。

其他的例子，在有父子结构的图形中：
```
this->addChild(new Sprite("picture.png"));
```

所谓Ownership便是指负责销毁对象的代码上下文。一般来说为创建对象的一方，但也有转交Ownership的情况，例如上面的addListener。


# 脚本难题 #

脚本语言都会有一套垃圾回收系统。所以在脚本语言中，所有由脚本创建的对象都会由脚本回收。
当Ownership还在创建该对象的上下文中，这样做没有问题。如果通过addListener将Ownership转交给C++上下文，那会怎么样？
按照Ownership定义，脚本没有必要持有该引用。当脚本没有该引用时，垃圾回收系统就会把它销毁，进而引发持有Ownership一端的对象持有一个无效的引用，或者，聪明些的代码仅仅会使得Listener消失掉而不至于程序崩溃。但这都不是我们想要的，例如如下的代码：
```
something.addListener(new Listener);
```
会在脚本执行过后一段时间Listener突然失效，这段时间长度取决于脚本什么时候想起垃圾回收。

## SWIG的解决方案 ##
SWIG的做法是为每个脚本中的对象添加一名成员own，当own为true的时候脚本执行垃圾回收的时候会销毁整个对象，否则仅仅销毁脚本对象的Wrapper，而保留其中的C++对象。
在调用某些函数会获取或丢掉Ownership，需要对own成员进行操作。

这算是一种较好的解决方案，能够在不改变C++定义的前提解决问题（非侵入式），是以增加脚本复杂性为代价的。

## 其他解决方案 ##
  * 对脚本任何操作都不转移Ownership。（相当于不解决，但可以减少系统复杂性）可以尝试用脚本语言再对C++导出符号做一层封装。
  * 使用引用计数，这样便没有明确的Ownership。但这样的话不支持引用计数的类型就不能够实现转移Ownership的策略。同时也增加了系统复杂度（众所周知，引用计数有环引用的问题）。
  * 令脚本的VALUE值被每个向脚本导出的对象所持有，然后可以调用脚本库的函数来阻止其被垃圾回收系统回收，这样又可以保证脚本与对象一一对应，（尽管SWIG中有trackobject的功能，但是能力比较有限，无法追踪导出对象在C++程序中被销毁的情况）可以保证高度统一。困难这样做很难得到SWIG的支持，其次会增加C++程序的难度，可能与导出语言耦合严重，而且同上，不支持的类型不能导出（这里指一些轻量级对象，如果全部支持的话可能会拖慢时间效率）。这样做最大的缺点是增加编写插件的难度。另外，该方法同引用计数一样，同样有环引用的问题，例如一个对象has-a一个脚本中定义的类的实例，并且实例引用了该对象，且对象的Ownership在脚本一端，这样两个对象都不能销毁。

# 万有引擎的Ownership #
svn checkout core/OUE/swig来获取接口文件。
VDISOWN typemap会使得传入参数失去Ownership，并保证参数在传入时拥有Ownership。
%newobject标记的函数会返回一个带Ownership的对象（其他的还有构造函数等）。如果一个对象又得到了Ownership是从函数返回的Wrapper带有的Ownership。（注：由于一些问题，一个C++对象可能对应多个脚本对象，这些脚本对象并不相等，可以理解为C++对象的多个Wrapper，在上述情况，调用者的Wrapper并不会获得Ownership，而是返回一个带Ownership的Wrapper）

如果一个函数没有VDISOWN标记，又不持有某个参数的引用，则该参数引用对象可能会被销毁。
如果%newobject标记的函数返回值不被赋值到某个脚本变量时，该对象可能会被回收。

这是脚本中的C++残留内存管理问题。