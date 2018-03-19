# LCUI ChangeLog

## 1.0.0 Beta 2 (2018-03-18)

### Bug Fixes

* some memory leak bugs ([#135](https://github.com/lc-soft/LCUI/issues/135)) ([9995b23](https://github.com/lc-soft/LCUI/commit/9995b23))
* **gui:** widget auto size computation bug ([095f4b8](https://github.com/lc-soft/LCUI/commit/095f4b8))
* **renderer:** incorrect widget content rectangle computation ([#122](https://github.com/lc-soft/LCUI/issues/122),[#123](https://github.com/lc-soft/LCUI/issues/123)) ([0f81863](https://github.com/lc-soft/LCUI/commit/0f81863))
* **renderer:** widget content overflow ([#144](https://github.com/lc-soft/LCUI/issues/144)) ([2a923a6](https://github.com/lc-soft/LCUI/commit/2a923a6))
* **thread:** memory leak bug when running test ([402bc03](https://github.com/lc-soft/LCUI/commit/402bc03))
* **widget:** scrollbar did not cancel the event bubbling ([#145](https://github.com/lc-soft/LCUI/issues/145)) ([7dd60ac](https://github.com/lc-soft/LCUI/commit/7dd60ac))
* **widget:** Scrollbar_BindBox() not working after scrollbar initialize ([196f47c](https://github.com/lc-soft/LCUI/commit/196f47c))
* **widget:** Widget_GetOffset() result does not include padding spacing ([0a893f4](https://github.com/lc-soft/LCUI/commit/0a893f4))
* **widget-event:** the touch point coordinate are not been converted ([a40eda2](https://github.com/lc-soft/LCUI/commit/a40eda2))
* **worker:** worker did not enter the blocked state ([#134](https://github.com/lc-soft/LCUI/issues/134)) ([8dae96f](https://github.com/lc-soft/LCUI/commit/8dae96f))

### Features

* **css:** add parsing support for "border-left: 0;" ([30de5b8](https://github.com/lc-soft/LCUI/commit/30de5b8))
* **timer:** add LCUITimer_SetTimeout() and LCUITimer_SetInterval() ([f032f6f](https://github.com/lc-soft/LCUI/commit/f032f6f))
* **widget-event:** add "link" event, rename "remove" event to "unlink" ([8c1d105](https://github.com/lc-soft/LCUI/commit/8c1d105))

### Performance Improvements

* **widget:** rename scrollbar widget attributes ([d397914](https://github.com/lc-soft/LCUI/commit/d397914))

## 1.0.0 Beta (2018-02-07)

### Features

- **font:** add support for font-weight and font-style selection
- **textlayer:** add `[bgcolor]` tag support (#110)
- **textlayer:** add `[i]` tag support (#115)
- **textlayer:** add `[b]` tag support (#112)
- **textlayer:** using FreeType to get space width
- **widget:** add content trimming support for textview (#97)
- **widget:** add anchor widget (#105)
- **widget:** add word-break property support for textview (#71)
- **widget:** add `WTT_RESIZE_WITH_SURFACE` task for sync surface size (#104)
- **widget:** add Widget_UnbindEventByHandlerId() function
- **css:** add css font-face rule parser (#95)
- **css:** add relative path support for url() parser (#98)
- **css:** add css property parser for parsing justify-content (#117)
- **css:** add support to parse "border: 0"
- **css:** add support to parse "line-height: 1"
- **layout:** add simple support for flex display and justify-content (#117)
- **bulder:** add short name `<w>` for `<widget>` (#103)
- **core:** add LCUIWorker for async task support (#106)

### Bug fixes

- **textlayer:** text vertical position is not center (#100)
- **widget:** textview height computation error (#111)
- **css:** stylesheet selection bug (#113)
- **graphic:** segmentation fault bug in Graph_FillRectARGB() (#110)
- **linux:** set "--enable-video-ouput=no" will make the linker output errors
- **renderer:** widget border rendering is incomplete when position is non-integer (#108)

### Improvements

- **layout:** improve widget size computation
- **font:** improve font data storage and operation method
- **mainloop:** set run all tasks in one frame
- **mainloop:** set the widget tree to update least once per frame

You can view [this pull request](https://github.com/lc-soft/LCUI/pull/120). for detailed code changes.

## CHANGES BETWEEN 1.0 alpha and 1.0 alpha2

- Adjust the code structure
- Improved TextView widget
- Improved caret positioning and text rendering for TextEdit widget
- Improved widget box-shadow rendering
- Add drivers for UWP Application, [here](https://github.com/lc-soft/LCUI/blob/develop/build/windows/LCUIApp/App.cpp) is example
- Improved support for high-resolution screen
  - Add dp and sp unit, Similar to dp and sp unit in Android
  - Add support scaling, It is possible to set the appropriate zoom ratio according to the screen pixel density
- Improve automation build and test
  - Add unit testing
  - Add code coverage testing
  - Enabled the [valgrind](http://valgrind.org/) memory check tool
- Fixed all memory leak BUGs and memory access errors
- Fixed some widget layout bugs
- Fixed jpeg reader sometimes fail to read data
- Fixed other known bugs

You can view [this pull request](https://github.com/lc-soft/LCUI/pull/83). for detailed code changes.

## CHANGES BETWEEN 0.15.0 and 1.0 alpha

- Improved bmp, jpeg, png file processing, add image reader
- Added Scrollbar widget
- Improved Button widget
- Improved TextView widget
- Improved TextEdit widget
- Improved widget types, events, layouts and styles processing
- Imrpoved UI rendering performance
- Added touch screen support
- Added XML and CSS support
- Removed Linux Framebuffer support
- Added Linux XWindow support
- Added simple support for Universal Windows Platform (UWP)
- Imrpoved Windows support
- Added some tutorials (only Chinese version)
- Fixed some bugs

## CHANGES BETWEEN 0.14.0 and 0.15.0

- Limit the graphical render maximum frames per second to 100, for reduce CPU usage.
- Modify event processing module, add an event disconnection functions, partial function has been modified.
- Update timer function modules, fix known BUG.
- Update TextBox and Label GUI Widget, optimized text rendering speed, fix known BUG.
- Add text auto-wrap function, if edit text in TextBox, may be problems when enabled auto-wrap function.
- Fixed: multi-threaded rendering text causes text bitmap abnormal, program crash and other problems, now the text rendering tasks has been transferred to the main thread performed separately.
- Fixed: known BUG in GUI widgets stacking order processing function.
- Fixed: all messages are not all processed when each processing GUI Widget messages. 

This updates mainly aimed author's LCUI 2D Fighting Game, since the LCUI 2D Fighting Game is mainly on the windows platform for development and testing, therefore, although some function modules on the windows platform support is good, but is not guaranteed can be normal support  GNU / Linux platform.


## CHANGES BETWEEN 0.13.0 and 0.14.0

- Improved support for windows platform, optimize the graphics output,
 improving the keyboard and mouse input support.
- You can set the video output mode, but valid only in the windows platform.
- Solve some of the modules thread-safety issues.
- Optimize GUI-Widget graphics resources memory usage.
- Improved GUI-Widget message processing.
- Improved GUI-Widget display order processing.
- Adjusted GUI-Widget click event and dragging event handling.
- GUI-Widget support by setting z-index value to adjust the display order.
- Add input method framework and default input method, currently only supports
 input English letters, numbers and symbols.
- For built-in bitmap font file (for example: simsun.ttc), is now able to
 normally display the font bitmap.
- Solve other BUGS.


## CHANGES BETWEEN 0.12.6 and 0.13.0

- Most of modules function naming style has been changed.
- Solve Multi-window display ordering issues.
- Improve GUI Widget processing module.
- Add event loop, improve GUI Widget event mechanism.
- Improve thread management module, Package linux and win32 thread 
  management and mutex related functions.
- Add win32 graphics output module , due to the limited level, 
  currently could only output graphics in the client area of the 
  window, needs to be improved.
- Add font management module, improve font bitmap processing module,
  optimize the loading speed of bitmap fonts, reduce the memory overhead.
- Add GraphLayer module, used to manage each widget graphics layer,
  LCUI video output mainly generated by the module processing.
- Add placeholder function for the TextBox Widget.
- Add MessageBox.
- Solve other BUGS.

Now, we are achieve the LCUI running on win32, you can use 
Visual Studio 2012 to directly compile this project source code, 
but exist some problems that need to be improved.


## CHANGES BETWEEN 0.12.5 and 0.12.6

- LCUI class renamed to LCUIApp, some widget C++ class changed to 
  inherited from the LCUIWidget class, cancel previous nested class.
- Re-planning project source code, and prepare for the future of 
  cross-platform.
- Add new widget: TextBox and ScrollBar, the TextBox also only have 
  the basic functions, pending to continue to improve.
- Add TextLayer module for handling text-bitmap drawing.
- The default style of window has been modified.
- Add timer function, ActvieBox widget animation each frame update 
  has been changed to use the timer.
- widget can get focus, you can connect FocusIn and FocusOut event 
  for response to widget get/lose focus.
- Keyboard input event mechanism has been modified, when keyboard 
  input, keyboard event default sent to the Focus Widget.
- Re-planning widget attributes, the widget update processing function 
  has been modified.
- Add new widget attributes: padding, you can adjust the padding 
  of the widget.
- Add new widget attributes: dock, you can set Widget's dock type.
- Add STATIC positioning type for widget, this positioning type of 
  widget will according to display order Auto-Arrange.
- Widget size support expressed as percentage, When the container 
  size has been changed, the widget's size will also change.


## CHANGES BETWEEN 0.12.4 and 0.12.5

- Add C + + class to some function modules, please see the 
  helloworld2.cpp file for more details.
- Add ActiveBox Widget, it can achieve the continuity of a simple
  multi-frame image switching display.
- Improved screen graphics refresh method.
- Solve the problem of disappear  after dragging the child window,  
  the reason is that the sub window position calculation is error.
- Optimized Alpha blending processing, efficiency has improved.
- Solve some of the problems of graphics processing.
- LCUI_Graph support the "quote" function, you can quote graph 
  of a specified area in an graph.
- Solve some of the problems of the PictureBox Widget.
- Improved the nested multi Widget graphic display processing.
- Improved the area refresh processing of widget moving.
- Some settings support environment variable setting.
- Solve the thread is blocked waiting for unlock the rwlock 
  problem, the problem is in the LCUI exit.


## CHANGES BETWEEN 0.12.3 and 0.12.4

- corrected the graphical display problems of the widget nesting. 
- the graphical display method has been changed, in theory,
  graphics refresh rate will be enhanced.
- some functions and data structures has been modified.
- add the RadioButton widget and CheckBox widget.
- the event mechanism has been modified, add mouse move / click 
  event and widget drag event.
- add graphics data read-write lock function to protect data, 
  prevent multiple threads simultaneously read / write.
- the header file has been modified, use LCUI library function, 
  you may need to include the appropriate header file.
- the graphical output function has been modified, not only 
  support 24 and 32-bit color, but also support on other color 
  monitor to display graphics.
- some source file has been merged.
- configure.ac and the README file has been modified.

