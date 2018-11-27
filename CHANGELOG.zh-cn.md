# [1.0.0-beta.4](https://github.com/lc-soft/LCUI/compare/v1.0.0-beta.3...v1.0.0-beta.4) (2018-11-19)

### 问题修复

* **dirent:** LCUI_ReadDirW() UNINITIALIZED READ ([dd610ad](https://github.com/lc-soft/LCUI/commit/dd610ad))
* **display:** 调整窗口尺寸时会暂停渲染 ([#164](https://github.com/lc-soft/LCUI/issues/164)) ([8ad667a](https://github.com/lc-soft/LCUI/commit/8ad667a))
* **display:** 一帧内有过多的重复的脏矩形 ([b5d9040](https://github.com/lc-soft/LCUI/commit/b5d9040))
* **font:** 在加载完字体文件后设置默认字体会失败 ([9b4c005](https://github.com/lc-soft/LCUI/commit/9b4c005))
* **font:** 在模块被重新初始化后，字体编号未重置 ([d75b4ca](https://github.com/lc-soft/LCUI/commit/d75b4ca))
* **gui:** 当部件的显示角色改变后应该更新布局 ([96c1cef](https://github.com/lc-soft/LCUI/commit/96c1cef))
* **gui:** 当模块被销毁时应该清空废弃的部件 ([1ad6be1](https://github.com/lc-soft/LCUI/commit/1ad6be1))
* **gui:** 块级元素的默认宽度应该为 100% ([adcb9e7](https://github.com/lc-soft/LCUI/commit/adcb9e7))
* **gui:** 在销毁部件前应该解除结点 ([3249490](https://github.com/lc-soft/LCUI/commit/3249490))
* **gui:** unlink 事件被重复触发 ([89faa5d](https://github.com/lc-soft/LCUI/commit/89faa5d))
* **gui:** 部件背景图未渲染 ([e76f3f9](https://github.com/lc-soft/LCUI/commit/e76f3f9))
* **gui:** Widget_AutoSize() 不应该改变静态的宽度或高度 ([8bee9c2](https://github.com/lc-soft/LCUI/commit/8bee9c2))
* **ime:** 无法正确识别非 qwerty 键盘布局的按键输入 ([#147](https://github.com/lc-soft/LCUI/issues/147)) ([4b1f050](https://github.com/lc-soft/LCUI/commit/4b1f050))
* **layout:** "left: auto" 被计算为 "left: 0" ([f4990da](https://github.com/lc-soft/LCUI/commit/f4990da))
* **platform:** linux 帧缓存驱动未被销毁 ([#157](https://github.com/lc-soft/LCUI/issues/157)) ([87f79f0](https://github.com/lc-soft/LCUI/commit/87f79f0))
* **renderer:** 当部件整体透明时，子级部件未正确渲染 ([#160](https://github.com/lc-soft/LCUI/issues/160)) ([d13b554](https://github.com/lc-soft/LCUI/commit/d13b554))
* **renderer:** 有透明效果的部件，渲染不正确 ([6668165](https://github.com/lc-soft/LCUI/commit/6668165))
* **textlayer:** 中文文本断行错误 ([1e5a262](https://github.com/lc-soft/LCUI/commit/1e5a262))
* **textlayer:** isalpha() 断言失败 ([cd39a46](https://github.com/lc-soft/LCUI/commit/cd39a46))
* **textview:** 在改变 "content" 属性后没有效果 ([d1ab50d](https://github.com/lc-soft/LCUI/commit/d1ab50d))
* **textview:** 设置文本后未更新自身尺寸 ([31e0582](https://github.com/lc-soft/LCUI/commit/31e0582))
* **util:** LCUI_DirEntry::name 的值不正确 ([367febe](https://github.com/lc-soft/LCUI/commit/367febe))
* **worker:** 工作线程内存访问越界 ([5a7e2c0](https://github.com/lc-soft/LCUI/commit/5a7e2c0))
* 当不支持 jpeg 和 png 时连接器会报错 ([2bbabe1](https://github.com/lc-soft/LCUI/commit/2bbabe1))
* PACKAGE_VERSION 未定义 ([70660de](https://github.com/lc-soft/LCUI/commit/70660de))
* 当系统中存在窗口管理系统时应该隐藏鼠标指针 ([acf9454](https://github.com/lc-soft/LCUI/commit/acf9454))
* LCUI_GetAppId() 返回值不正确 ([46ec607](https://github.com/lc-soft/LCUI/commit/46ec607))
* 在 FreeBSD 上编译时，"linux/input.h" 文件不存在 ([7d95f3f](https://github.com/lc-soft/LCUI/commit/7d95f3f))
* 在 FreeBSD 上编译时，ENODATA 未定义 ([2cd0b27](https://github.com/lc-soft/LCUI/commit/2cd0b27))
* 未解析的外部符号 _Graph_IsValid ([1328a2d](https://github.com/lc-soft/LCUI/commit/1328a2d))

### 新特性

* **builder:** `<resource>` 支持加载 xml 文件 ([d5e162c](https://github.com/lc-soft/LCUI/commit/d5e162c))
* **display:** 自动禁用窗口最大化按钮 ([#164](https://github.com/lc-soft/LCUI/issues/164)) ([82fad1b](https://github.com/lc-soft/LCUI/commit/82fad1b))
* **event:** 添加 ctrl_key 和 shift_key 成员至 LCUI_KeyboardEvent ([6f5f17c](https://github.com/lc-soft/LCUI/commit/6f5f17c))
* **gui:** 添加 Widget_SetOpacity() ([e6ad163](https://github.com/lc-soft/LCUI/commit/e6ad163))
* **gui:** 添加 Widget_SetVisibility() ([145d9ee](https://github.com/lc-soft/LCUI/commit/145d9ee))
* **gui:** 添加 Widget_SetVisible() 和 Widget_SetHidden() ([2f581b8](https://github.com/lc-soft/LCUI/commit/2f581b8))
* **gui:** LCUIWidget_ClearTrash() 返回值为实际销毁的部件数量 ([6262f20](https://github.com/lc-soft/LCUI/commit/6262f20))
* **linux:** 为 x11 系统添加鼠标滚轮事件处理 ([#54](https://github.com/lc-soft/LCUI/issues/54)) ([1061592](https://github.com/lc-soft/LCUI/commit/1061592))
* **linux:** 使用 fontconfig 定位字体文件路径 ([2932246](https://github.com/lc-soft/LCUI/commit/2932246))
* **textview:** 在加载新的字体文件后刷新所有 textview 部件 ([3fcfa99](https://github.com/lc-soft/LCUI/commit/3fcfa99))
* **util:** 添加 OpenUri() ([ec20c99](https://github.com/lc-soft/LCUI/commit/ec20c99))
* 添加 LCUI_MAX_FRAMES_PER_SEC 宏定义 ([6fa2995](https://github.com/lc-soft/LCUI/commit/6fa2995))
* 添加 LCUI_MAX_FRAME_MSEC 宏定义 ([7abc901](https://github.com/lc-soft/LCUI/commit/7abc901))

### 改进

* **charset:** 改进 utf-8 和 unicode 字符串转换 ([1efd856](https://github.com/lc-soft/LCUI/commit/1efd856))
* **graph:** 添加新的图片缩放方式 (issue [#39](https://github.com/lc-soft/LCUI/issues/39)) ([bbfb9af](https://github.com/lc-soft/LCUI/commit/bbfb9af))

## 1.0.0 Beta 3 (2018-05-08)

### 问题修复

* **font:** 因字体缓存位置计算错误而导致的 DeleteFont() 段错误 ([5465c6b](https://github.com/lc-soft/LCUI/commit/5465c6b))
* **timer:** 定时器线程在被创建后可能会立刻退出 ([0b01f88](https://github.com/lc-soft/LCUI/commit/0b01f88))
* **util:** 在 Unix 系统下编译会报错：'struct dirent' has no member named 'd_reclen' ([#141](https://github.com/lc-soft/LCUI/issues/141)) ([0416c42](https://github.com/lc-soft/LCUI/commit/0416c42))

### 新特性

* **platform:** 添加 linux 平台的帧缓存（FrameBuffer）驱动 ([6015838](https://github.com/lc-soft/LCUI/commit/6015838))
* **platform:** 添加 linux 平台的键盘驱动 ([ad3348e](https://github.com/lc-soft/LCUI/commit/ad3348e))
* **platform:** 添加 linux 平台的鼠标驱动 ([0ff7b70](https://github.com/lc-soft/LCUI/commit/0ff7b70))

## 1.0.0 Beta 2 (2018-03-18)

### 问题修复

* 一些内存泄露问题 ([#135](https://github.com/lc-soft/LCUI/issues/135)) ([9995b23](https://github.com/lc-soft/LCUI/commit/9995b23))
* **gui:** 部件大小计算错误 ([095f4b8](https://github.com/lc-soft/LCUI/commit/095f4b8))
* **renderer:** 部件内容区域计算错误 ([#122](https://github.com/lc-soft/LCUI/issues/122),[#123](https://github.com/lc-soft/LCUI/issues/123)) ([0f81863](https://github.com/lc-soft/LCUI/commit/0f81863))
* **renderer:** 部件内容溢出 ([#144](https://github.com/lc-soft/LCUI/issues/144)) ([2a923a6](https://github.com/lc-soft/LCUI/commit/2a923a6))
* **thread:** 线程在退出后未释放之前申请的内存资源 ([402bc03](https://github.com/lc-soft/LCUI/commit/402bc03))
* **widget:** 滚动条未取消事件冒泡 ([#145](https://github.com/lc-soft/LCUI/issues/145)) ([7dd60ac](https://github.com/lc-soft/LCUI/commit/7dd60ac))
* **widget:** Scrollbar_BindBox() 在滚动条初始化后未能正常工作 ([196f47c](https://github.com/lc-soft/LCUI/commit/196f47c))
* **widget:** Widget_GetOffset() 返回的结果未包含内间距 ([0a893f4](https://github.com/lc-soft/LCUI/commit/0a893f4))
* **widget-event:** 部件触控事件中的触点坐标未根据全局缩放比例进行转换 ([a40eda2](https://github.com/lc-soft/LCUI/commit/a40eda2))
* **worker:** 工作线程获取新任务前未进入阻塞状态 ([#134](https://github.com/lc-soft/LCUI/issues/134)) ([8dae96f](https://github.com/lc-soft/LCUI/commit/8dae96f))

### 新特性

* **css:** 添加支持解析 "border-left: 0;" ([30de5b8](https://github.com/lc-soft/LCUI/commit/30de5b8))
* **timer:** 添加更具语义的 LCUITimer_SetTimeout() 和 LCUITimer_SetInterval() ([f032f6f](https://github.com/lc-soft/LCUI/commit/f032f6f))
* **widget-event:** 添加 "link" 事件, 重命名 "remove" 事件为 "unlink" ([8c1d105](https://github.com/lc-soft/LCUI/commit/8c1d105))

### 改进

* **widget:** 重命名滚动条的属性名称和 CSS 选择符 ([d397914](https://github.com/lc-soft/LCUI/commit/d397914))

## 1.0 Beta (2018-02-07)

### 新特性

- **font:** 添加支持按字重（font-weight）和风格（font-style）选择字体
- **textlayer:** 添加 `[bgcolor]` 标签支持，用于设置文本背景色 (#110)
- **textlayer:** 添加 `[i]` 标签支持，用于设置文本为斜体 (#115)
- **textlayer:** 添加 `[b]` 标签支持，用于设置文本为粗体 (#112)
- **textlayer:** 使用 FreeType 字体引擎默认的方式获取空格宽度
- **widget:** 为 textview 添加内容修剪支持，启用后会去除文本首尾空白符 (#97)
- **widget:** 添加锚点（anchor）组件 (#105)
- **widget:** 为 textview 添加 word-break 属性支持，用于设置单词断行方式 (#71)
- **widget:** 添加 `WTT_RESIZE_WITH_SURFACE` 任务类型，用于将表面（surface）大小与部件同步 (#104)
- **widget:** 添加 Widget_UnbindEventByHandlerId() 函数，用于根据事件处理器标识号取消事件绑定
- **css:** 添加 font-face 规则解析器，可用于在 css 代码中定义字体信息 (#95)
- **css:** 为 url() 的解析器添加支持处理相对路径 (#98)
- **css:** 添加针对 justify-content 的属性解析器 (#117)
- **css:** 为 border 的属性解析器添加支持解析 "border: 0"
- **css:** 为 line-height 的属性解析器添加支持解析 "line-height: 1"
- **layout:** 添加简单的 flex 显示方式和 justify-content 属性支持 (#117)
- **bulder:** 为 `<widget>` 添加短名称 `<w>` (#103)
- **core:** 添加 LCUIWorker 用于处理异步任务 (#106)

### 问题修复

- **textlayer:** 文本没有垂直居中对齐 (#100)
- **widget:** textview 的高度计算错误 (#111)
- **css:** 样式表选取问题，同名选择器会共用同一优先级 (#113)
- **graphic:** Graph_FillRectARGB() 段错误 (#110)
- **linux:** 设置 "--enable-video-ouput=no" 选项后会导致链接器报错
- **renderer:** 部件位置为非整数时，在渲染时边框会时有时无 (#108)

### 改进

- **layout:** 改进部件尺寸计算
- **layout:** 改进块（block）和内联块（inline-block）元素的布局处理
- **font:** 改进字体数据的存储和操作方式
- **mainloop:** 设置每帧运行任务队列中的所有任务
- **mainloop:** 设置每帧至少更新一次部件树

你可以查看此[拉取请求](https://github.com/lc-soft/LCUI/pull/120)来了解详细的代码变更记录。

## 1.0 alpha 和 1.0 alpha2 之间的变化

- 调整代码结构
- 改进 TextView 部件
- 改进 TextEdit 部件的光标定位与文本渲染
- 改进部件的盒形阴影（box-shadow）绘制
- 添加适用于 UWP 应用的驱动支持，示例应用代码[在这里](https://github.com/lc-soft/LCUI/blob/develop/build/windows/LCUIApp/App.cpp)。
- 改进对高分屏的支持
  - 添加 dp、sp 度量单位，功能与 Android 中的同名单位相似
  - 添加缩放支持，可根据屏幕像素密度设置合适的缩放比例
- 完善自动化构建和测试
  - 添加单元测试
  - 添加代码覆盖率测试
  - 引入 valgrind 内存检查工具
- 解决所有内存泄露和内存访问越界问题
- 解决部件布局功能中存在的一些问题
- 解决 jpeg 图像读取器有时会读取失败的问题
- 其它已知 BUG 修复

你可以查看此[拉取请求](https://github.com/lc-soft/LCUI/pull/83)来了解详细的代码变更记录。

## 0.15.0 和 1.0 alpha 之间的变化

- 改进 bmp、jpeg、png 图片的读取接口
- 添加滚动条（Scrollbar）部件
- 改进按钮（Button）部件
- 改进文本显示（TextView）部件
- 改进文本编辑（TextEdit）部件
- 改进部件的类型、事件、布局及样式处理
- 改进图形界面的性能
- 添加触控支持
- 添加 CSS 和 XML 支持
- 移除 Linux 的帧缓冲（FrameBuffer）支持
- 添加对 Linux 的 XWindow 的支持
- 添加对 Windows 通用应用平台（UWP）的简单支持
- 改进对 Windows 的支持
- 添加部分文档
- 修复部分已知BUG

## 0.14.0 和 0.15.0 之间的变化

- 限制每秒图形更新帧数最大为100，减少CPU占用。
- 调整事件处理模块，添加事件连接的解除功能，部分函数接口做了修改。
- 更新定时器功能模块，修复已知的BUG。
- 更新TextBox和Label部件，优化文本绘制速度，修复部分已知BUG。
- 添加文本的自动换行功能，但仅适用于静态文本显示，若在已启用自动换行功能的TextBox部件中进行文本编辑，可能会出现问题。
- 解决多线程进行文本渲染时导致的文本位图异常、程序崩溃等的问题，现已将文本渲染任务转由主线程单独执行。
- 修复GUI部件的堆叠顺序处理功能中的已知BUG。
- 解决在每次处理GUI部件消息时未完全处理掉全部消息的问题。

本次更新主要针对作者应用LCUI开发的2D格斗游戏，LCUI中的部分已知BUG并未解决，作者计划会在后续版本中修复这些BUG。由于该游戏主要在windows平台上进行开发和测试，因此，虽然部分功能模块对windows平台支持良好，但并不保证也能正常支持GNU/Linux平台。


## 0.13.0 和 0.14.0 之间的变化

- 改进对windows平台的支持，优化了图形输出，改善了对键盘和鼠标的输入支持。
- 可设置视频输出模式，仅在windows平台下有效。
- 解决部分模块的线程安全问题。
- 优化部件图形资源的内存占用。
- 改进部件的消息处理。
- 完善部件的堆叠顺序处理。
- 调整部件的点击事件和拖动事件的处理。
- 部件支持通过设置z-index值来调整堆叠顺序。
- 修改window部件和button部件的风格。
- 添加输入法框架和默认的输入法，目前只支持输入英文字母、数字、符号。
- 对于内置点阵位图的字体文件（例如：宋体），现已能够正常显示字体的点阵位图。
- 解决其它的细节问题。


## 0.12.6 和 0.13.0 之间的变化

- 修改大部分模块的函数命名风格。
- 解决多窗口显示顺序异常的问题。
- 完善GUI部件处理模块。
- 添加事件循环，完善GUI部件的事件机制。
- 完善线程管理模块，封装了linux和win32中常用的线程管理及互斥锁相关的函数。
- 添加win32下的图形输出模块，由于水平有限，目前只能在窗口客户区内输出图形，有待完善。
- 添加字体管理模块，完善字体位图处理模块，优化字体位图的载入速度，减少内存开销。
- 新增GraphLayer模块，用于管理每个部件的图层，LCUI输出的图像主要由该模块处理生成。
- TextBox部件添加占位符功能。
- 添加MessageBox。
- 解决其它的细节问题。

目前已初步实现在windows系统上运行，可直接使用Visual Studio 2012编译此项目，但存在一些问题，有待完善。


## 0.12.5 和 0.12.6 之间的变化

- LCUI类改为LCUIApp类，部分部件的C++类继承自LCUIWidget类，取消之前的类嵌套。
- 重新规划项目源代码，为以后的跨平台、适应不同环境而做准备。
- 新增文本框和滚动条部件，文本框还只具备基本功能，有待继续完善。
- 新增TextLayer模块，用于处理文字位图的绘制。
- 修改label部件代码，label部件和文本框部件的文本位图的绘制都由TextLayer模块实现。
- 修改window部件的默认风格。
- 添加定时器，ActvieBox部件的动画每帧图像的定时更新，改用定时器实现。
- 部件能够获得焦点，可关联FocusIn和FocusOut事件，以在部件获得/失去焦点时作出响应。
- 修改按键输入事件机制，当用键盘输入内容时，按键事件默认发送至已获得焦点的部件。
- 重新规划部件的属性，修改部件的更新处理功能。
- 部件添加padding属性，可调整部件的内边距。
- 部件添加STATIC定位类型，该定位类型的部件将根据显示顺序，自动排列。
- 部件添加dock属性，可设置部件的停靠类型。
- 部件尺寸支持百分比表示，当容器尺寸改变后，该部件的尺寸会随着改变。
	

## 0.12.4 和 0.12.5 之间的变化

- 部分功能模块添加C++类，可使用C++的方式调用，具体请参考helloworld2.cpp的内容。
- 添加ActiveBox部件，用它可实现简单的多帧图像的连贯切换显示。
- 完善了屏幕图形的刷新方法。
- 纠正在嵌套多层窗口时，拖动子窗口消失的问题，原因是子窗口位置计算错误。
- 优化了图形的Alpha混合处理，效率有所提升。
- 解决图像处理的一些问题。
- 图像支持“引用”功能，可引用一个图像中的指定区域的图形数据，无需从图像中裁剪。
- 解决了在PictureBox部件尺寸不大于0时所出现的一些问题。
- 在PictureBox部件未设定图像的情况下，对其设置尺寸模式时不会再出现段错误。
- 完善多部件嵌套时的图形显示处理。
- 完善部件移动时的区域刷新处理。
- 字体文件位置、FrameBuffer设备、等支持用环境变量设置。
- 解决线程阻塞等待自己解锁读写锁的问题，该问题在LCUI退出时出现。
	

## 0.12.3 和 0.12.4 之间的变化

- 纠正了嵌套多个部件所产生的图形显示上的问题。 
- 更改了图形显示处理方式，理论上图形刷新速度会有所提升。
- 部分函数以及数据结构做了修改。
- 新增RadioButton部件和CheckBox部件。
- 事件机制做了修改，添加 鼠标移动/点击事件 和 部件拖动事件 的关联功能。
- 图形数据添加了读写锁功能，以保护数据，避免多个线程对其同时进行读/写。
- 头文件做了修改，使用LCUI库的函数时，可能需要包含相应的头文件。
- 图形输出功能做了修改，支持在非24和32位色的显示器上显示图形。
- 合并了部分源码文件中的源代码。
- configure.ac以及README文件做了修改。

