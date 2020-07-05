# [2.1.0](https://github.com/lc-soft/LCUI/compare/v2.0.0...v2.1.0) (2020-07-05)


### Bug Fixes

* wasm-ld: error: duplicate symbol: self ([2b0424c](https://github.com/lc-soft/LCUI/commit/2b0424c))
* **display:** rect_array is not freed ([224c433](https://github.com/lc-soft/LCUI/commit/224c433))
* **gui:** invalid area are not added after the widget is destroyed ([8e1f136](https://github.com/lc-soft/LCUI/commit/8e1f136))
* **gui:** segmention fault in TextView_SetTextW() ([a10df88](https://github.com/lc-soft/LCUI/commit/a10df88)), closes [#195](https://github.com/lc-soft/LCUI/issues/195)
* **ime:** segmentation fault when set caret ([0b644e1](https://github.com/lc-soft/LCUI/commit/0b644e1))
* **linux:** segmentation fault on draw window with minimal size ([#200](https://github.com/lc-soft/LCUI/issues/200)) ([#201](https://github.com/lc-soft/LCUI/issues/201)) ([fa23f89](https://github.com/lc-soft/LCUI/commit/fa23f89))
* **mainloop:** move out paint event trigger from openmp parallel loop ([#204](https://github.com/lc-soft/LCUI/issues/204)) ([#205](https://github.com/lc-soft/LCUI/issues/205)) ([6978f46](https://github.com/lc-soft/LCUI/commit/6978f46))


### Features

* add settings api ([#191](https://github.com/lc-soft/LCUI/issues/191)) ([#211](https://github.com/lc-soft/LCUI/issues/211)) ([#212](https://github.com/lc-soft/LCUI/issues/212)) ([1b8c6e6](https://github.com/lc-soft/LCUI/commit/1b8c6e6))



# [2.1.0](https://github.com/lc-soft/LCUI/compare/v2.0.0...v2.1.0) (2020-07-05)


### Bug Fixes

* wasm-ld: error: duplicate symbol: self ([2b0424c](https://github.com/lc-soft/LCUI/commit/2b0424c))
* **display:** rect_array is not freed ([224c433](https://github.com/lc-soft/LCUI/commit/224c433))
* **gui:** invalid area are not added after the widget is destroyed ([8e1f136](https://github.com/lc-soft/LCUI/commit/8e1f136))
* **gui:** segmention fault in TextView_SetTextW() ([a10df88](https://github.com/lc-soft/LCUI/commit/a10df88)), closes [#195](https://github.com/lc-soft/LCUI/issues/195)
* **ime:** segmentation fault when set caret ([0b644e1](https://github.com/lc-soft/LCUI/commit/0b644e1))
* **linux:** segmentation fault on draw window with minimal size ([#200](https://github.com/lc-soft/LCUI/issues/200)) ([#201](https://github.com/lc-soft/LCUI/issues/201)) ([fa23f89](https://github.com/lc-soft/LCUI/commit/fa23f89))
* **mainloop:** move out paint event trigger from openmp parallel loop ([#204](https://github.com/lc-soft/LCUI/issues/204)) ([#205](https://github.com/lc-soft/LCUI/issues/205)) ([6978f46](https://github.com/lc-soft/LCUI/commit/6978f46))



# [2.0.0](https://github.com/lc-soft/LCUI/compare/v1.3.0...v2.0.0) (2020-03-02)


### Bug Fixes

* **builder:** comment node should be ignore ([460ee00](https://github.com/lc-soft/LCUI/commit/460ee00))
* **css:** conditional jump or move depends on uninitialised value(s) ([44486f1](https://github.com/lc-soft/LCUI/commit/44486f1))
* **font:** fix to get the correct file path ([#187](https://github.com/lc-soft/LCUI/issues/187)) ([6d54685](https://github.com/lc-soft/LCUI/commit/6d54685))
* **font:** FontBitmap_Free() memory leak ([c47a6c0](https://github.com/lc-soft/LCUI/commit/c47a6c0))
* **gui:** root widget is missing hover and active status ([02c03c7](https://github.com/lc-soft/LCUI/commit/02c03c7))
* **gui:** segmentation fault on resize empty window ([#199](https://github.com/lc-soft/LCUI/issues/199)) ([56ce0b5](https://github.com/lc-soft/LCUI/commit/56ce0b5))
* **gui:** TextCaret did not remove the timer after destruction ([a58b12d](https://github.com/lc-soft/LCUI/commit/a58b12d))
* **linux:** InitLinuxKeybord() memory leak bug ([61cadc8](https://github.com/lc-soft/LCUI/commit/61cadc8))
* **timer:** the callback will still run after the timer is removed ([43233b3](https://github.com/lc-soft/LCUI/commit/43233b3))
* correct the use of variables ([513b3b9](https://github.com/lc-soft/LCUI/commit/513b3b9))
* **windows:** touch support is not enabled by default ([bc7710a](https://github.com/lc-soft/LCUI/commit/bc7710a))
* cursor position should be updated before handle widget events ([83fc949](https://github.com/lc-soft/LCUI/commit/83fc949))
* **util:** the preset dict types are not exported correctly ([20abb19](https://github.com/lc-soft/LCUI/commit/20abb19))


### Code Refactoring

* **gui:** add widget_background.h ([d69fbb0](https://github.com/lc-soft/LCUI/commit/d69fbb0))
* **gui:** add widget_border.h ([843232e](https://github.com/lc-soft/LCUI/commit/843232e))
* **gui:** add widget_shadow.h ([08ed51c](https://github.com/lc-soft/LCUI/commit/08ed51c))
* **gui:** improve widget update processing ([1a50aec](https://github.com/lc-soft/LCUI/commit/1a50aec))


### Features

* **builder:** output details when an error occurs ([f7ed3b8](https://github.com/lc-soft/LCUI/commit/f7ed3b8))
* **css:** add flexbox property parser ([07d2911](https://github.com/lc-soft/LCUI/commit/07d2911))
* **display:** add LCUIDisplay_EnablePaintFlashing() ([298ffa4](https://github.com/lc-soft/LCUI/commit/298ffa4))
* **display:** flashing rendered rects ([#180](https://github.com/lc-soft/LCUI/issues/180)) ([#190](https://github.com/lc-soft/LCUI/issues/190)) ([5ad4fec](https://github.com/lc-soft/LCUI/commit/5ad4fec))
* **display:** set the minimum screen size to 320x240 ([317df70](https://github.com/lc-soft/LCUI/commit/317df70))
* **gui:** add CSSFontStyle_IsEquals() ([80d4149](https://github.com/lc-soft/LCUI/commit/80d4149))
* **gui:** add flexible box layout ([3cbb246](https://github.com/lc-soft/LCUI/commit/3cbb246))
* **gui:** change the call timing and parameter list for the runtask() ([f058916](https://github.com/lc-soft/LCUI/commit/f058916))
* **gui:** rewrite widget layout system ([24e89aa](https://github.com/lc-soft/LCUI/commit/24e89aa))
* **gui:** ScrollBar widget will reset when the container is destroyed ([61e0f2c](https://github.com/lc-soft/LCUI/commit/61e0f2c))
* **gui:** set the container padding when the ScrollBar is visible ([a436f41](https://github.com/lc-soft/LCUI/commit/a436f41))
* **gui:** unwrap() will fire the link and unlink events for each child ([48344bc](https://github.com/lc-soft/LCUI/commit/48344bc))
* **gui:** update the sizing rules for the TextView widget ([ea7e9d2](https://github.com/lc-soft/LCUI/commit/ea7e9d2))
* **util:** use inline functions instead of global variables ([0991d14](https://github.com/lc-soft/LCUI/commit/0991d14))


### Performance Improvements

* **display:** no need to add dirty rectangles after resizing ([b0985a2](https://github.com/lc-soft/LCUI/commit/b0985a2))
* **display:** OpenMP is enabled only when the render area is large ([1e57d9d](https://github.com/lc-soft/LCUI/commit/1e57d9d))
* **gui:** improve invalid area collection ([e9ea262](https://github.com/lc-soft/LCUI/commit/e9ea262))
* **gui:** improve textedit update processing ([bbb7cbc](https://github.com/lc-soft/LCUI/commit/bbb7cbc))
* **gui:** improve textview update processing ([6824735](https://github.com/lc-soft/LCUI/commit/6824735))
* add OpenMP support for widget rendering ([#118](https://github.com/lc-soft/LCUI/issues/118)) ([#189](https://github.com/lc-soft/LCUI/issues/189)) ([d858333](https://github.com/lc-soft/LCUI/commit/d858333))
* improve dirty rectangle collection ([c81da29](https://github.com/lc-soft/LCUI/commit/c81da29))


### BREAKING CHANGES

* **util:** `DictType_StringKey` and `DictType_StringCopyKey` have been replaced by inline functions
* **display:** LCUIDisplay_ShowRectBorder() LCUIDisplay_HideRectBorder() have been removed.
* **gui:** The widget shadow operation interface has been changed to private
* **gui:** The widget background operation interface has been changed to private
* **gui:** The widget border operation interface has been changed to private
* **gui:** The textview widget must operate in the UI thread.
* **gui:** The runtask() method takes two arguments and is called before each task is processed.
* **gui:** Some widget operation functions have been renamed or removed.




# [1.3.0](https://github.com/lc-soft/LCUI/compare/v1.2.0-beta...v1.3.0) (2019-10-07)

### Bug Fixes

* conflict with C++ "operator" keyword ([5a5ba8c](https://github.com/lc-soft/LCUI/commit/5a5ba8c))
* **css:** setting font style parser's style_handler doesn't work ([6869683](https://github.com/lc-soft/LCUI/commit/6869683))
* **display:** memory leak from X11Surface_SetCaptionW() ([484c3d7](https://github.com/lc-soft/LCUI/commit/484c3d7))
* **font:** failed to get bitmap when font_size > 18 ([d6315c5](https://github.com/lc-soft/LCUI/commit/d6315c5))
* **gui:** the default border color of the widget should be transparent ([5164955](https://github.com/lc-soft/LCUI/commit/5164955))
* **gui:** event target selection bug when widget pointer-events is none ([0f26c8b](https://github.com/lc-soft/LCUI/commit/0f26c8b))
* **gui:** TextEdit placeholder are not working ([d827767](https://github.com/lc-soft/LCUI/commit/d827767))
* **gui:** the mousemove event should fire before the mouseout event ([5020b91](https://github.com/lc-soft/LCUI/commit/5020b91))
* **gui:** widget attribute value should always be available ([4b0a2ed](https://github.com/lc-soft/LCUI/commit/4b0a2ed))
* **ime:** composition window position problem ([#36](https://github.com/lc-soft/LCUI/issues/36), [#175](https://github.com/lc-soft/LCUI/issues/175)) ([1107f91](https://github.com/lc-soft/LCUI/commit/1107f91))
* **renderer:** widget content rendering incorrect ([f8b0f8b](https://github.com/lc-soft/LCUI/commit/f8b0f8b))
* **util:** incorrect object type checking in Object_Operate() ([a326e8c](https://github.com/lc-soft/LCUI/commit/a326e8c))
* **util:** ParseUrl() parsing result is incorrect ([3f9450c](https://github.com/lc-soft/LCUI/commit/3f9450c))


### Features

* add LCUI_GetVersion() ([de40c7c](https://github.com/lc-soft/LCUI/commit/de40c7c))
* add rounded border rendering ([#174](https://github.com/lc-soft/LCUI/issues/174)) ([f36d071](https://github.com/lc-soft/LCUI/commit/f36d071))
* add include/LCUI.h file ([#173](https://github.com/lc-soft/LCUI/issues/173)) ([113af6a](https://github.com/lc-soft/LCUI/commit/113af6a))
* **builder:** tag name can be an existing widget type name ([4f6a01c](https://github.com/lc-soft/LCUI/commit/4f6a01c))
* **css:** make CSSFontStyle_* functions public ([568c915](https://github.com/lc-soft/LCUI/commit/568c915))
* **graph:** add LCUI_OverPixel() ([d8075d9](https://github.com/lc-soft/LCUI/commit/d8075d9))
* **gui:** add canvas widget ([e246843](https://github.com/lc-soft/LCUI/commit/e246843))
* **gui:** add Widget_CollectReferences() ([811585b](https://github.com/lc-soft/LCUI/commit/811585b))
* **gui:** add Widget_Each() ([2d7d1ee](https://github.com/lc-soft/LCUI/commit/2d7d1ee))
* **logger:** support setting logging level ([173b92f](https://github.com/lc-soft/LCUI/commit/173b92f))
* **scrollbar:** capture touch and mousewheel events from the container ([f2f9162](https://github.com/lc-soft/LCUI/commit/f2f9162))

# [1.2.0-beta](https://github.com/lc-soft/LCUI/compare/v1.1.0-beta...v1.2.0-beta) (2019-06-17)

### Bug Fixes

* **textlayer:** incorrect text line height calculation ([#169](https://github.com/lc-soft/LCUI/issues/169)) ([a4d2f0a](https://github.com/lc-soft/LCUI/commit/a4d2f0a))

### Features

* **gui:** add default methods for widget prototype ([e68e8e5](https://github.com/lc-soft/LCUI/commit/e68e8e5))
* **gui:** add TextEdit_BindProperty() ([c9d8ded](https://github.com/lc-soft/LCUI/commit/c9d8ded))
* **gui:** add Widget_BindProperty() ([0d3fb68](https://github.com/lc-soft/LCUI/commit/0d3fb68))
* **gui:** add Widget_SetStyleString() ([5163363](https://github.com/lc-soft/LCUI/commit/5163363))
* **gui:** add Widget_SetText() ([02714bf](https://github.com/lc-soft/LCUI/commit/02714bf))
* **textedit:** add TextEdit_GetProperty() ([512706e](https://github.com/lc-soft/LCUI/commit/512706e))
* **util:** add object wrapper and operation set ([241c652](https://github.com/lc-soft/LCUI/commit/241c652))
* **util:** add Object_ToString() ([13465c7](https://github.com/lc-soft/LCUI/commit/13465c7))

# [1.1.0-beta](https://github.com/lc-soft/LCUI/compare/v1.0.0-beta.4...v1.1.0-beta) (2019-03-10)


### Bug Fixes

* **builder:** widget should be initialized before appending ([f12e00b](https://github.com/lc-soft/LCUI/commit/f12e00b))
* **gui:** event target should be initialized ([ebbd128](https://github.com/lc-soft/LCUI/commit/ebbd128))
* **gui:** focus event and "focusable" property parsing bug ([d9a39a5](https://github.com/lc-soft/LCUI/commit/d9a39a5))
* **gui:** mouse events should be ignored when pointer-events is none ([fc54a64](https://github.com/lc-soft/LCUI/commit/fc54a64))
* **gui:** TextEdit should reset the caret position after blur ([8de4e71](https://github.com/lc-soft/LCUI/commit/8de4e71))
* **gui:** TextEdit_SetTextW() does not clear the previous content ([51eef68](https://github.com/lc-soft/LCUI/commit/51eef68))
* **gui:** the button height is different from the textedit ([5a918da](https://github.com/lc-soft/LCUI/commit/5a918da))
* **gui:** widget width with margin is incorrectly calculated ([#167](https://github.com/lc-soft/LCUI/issues/167)) ([d14023e](https://github.com/lc-soft/LCUI/commit/d14023e))
* **thread:** LCUIThread_Join() has not waited for the thread to exit ([4ddb833](https://github.com/lc-soft/LCUI/commit/4ddb833))
* **util:** decoded string length should not include terminator ([93f3d77](https://github.com/lc-soft/LCUI/commit/93f3d77))
* **worker:** task queue should not be blocked when running a task ([2b41f54](https://github.com/lc-soft/LCUI/commit/2b41f54))


### Features

* **gui:** add rule for allow widget updates only when visible ([93049c4](https://github.com/lc-soft/LCUI/commit/93049c4))
* **gui:** add rule for cache children style ([50cc6b2](https://github.com/lc-soft/LCUI/commit/50cc6b2))
* **gui:** add rule for first update visible children ([4cd6fdc](https://github.com/lc-soft/LCUI/commit/4cd6fdc))
* **gui:** add rule for limit the number of children rendered ([ec5e0d6](https://github.com/lc-soft/LCUI/commit/ec5e0d6))
* **gui:** add support for custom widget update rules ([bdd1d1c](https://github.com/lc-soft/LCUI/commit/bdd1d1c))
* **gui:** add widget border helper ([e8d52df](https://github.com/lc-soft/LCUI/commit/e8d52df))
* **gui:** add Widget_GetClosest() ([e740138](https://github.com/lc-soft/LCUI/commit/e740138))
* **gui:** add Widget_SetHashList() ([f466dee](https://github.com/lc-soft/LCUI/commit/f466dee))
* **gui:** auto assign an id to the event name ([fee31b0](https://github.com/lc-soft/LCUI/commit/fee31b0))
* **gui:** Widget_SetAttribute() will call proto->setattr() ([19992d1](https://github.com/lc-soft/LCUI/commit/19992d1))
* add LCUI_Profile for profiling performance ([f715a08](https://github.com/lc-soft/LCUI/commit/f715a08))
* **textview:** add TextView_SetColor() ([3a7b53f](https://github.com/lc-soft/LCUI/commit/3a7b53f))
* **util:** add return value for LCUIRect_ValidateArea() ([2fa6cf3](https://github.com/lc-soft/LCUI/commit/2fa6cf3))
* **util:** add strhash() ([ebf843f](https://github.com/lc-soft/LCUI/commit/ebf843f))
* **util:** add strpool ([c843df1](https://github.com/lc-soft/LCUI/commit/c843df1))
* **util:** add strreplace() ([409395d](https://github.com/lc-soft/LCUI/commit/409395d))
* **util:** default using OutputDebugString() to output log on Windows ([79ec21b](https://github.com/lc-soft/LCUI/commit/79ec21b))


### Performance Improvements

* **css:** change the stylesheet store struct to reduce memory usage ([9cc2957](https://github.com/lc-soft/LCUI/commit/9cc2957))
* **css:** reduce duplicate memory alloc for DictType ([01fd3f6](https://github.com/lc-soft/LCUI/commit/01fd3f6))
* **gui:** improve widget destruction performance ([87aff87](https://github.com/lc-soft/LCUI/commit/87aff87))
* **gui:** improve Widget_Empty() performance ([1654963](https://github.com/lc-soft/LCUI/commit/1654963))
* **gui:** reduce memory usage for widget style sheets ([3790fbd](https://github.com/lc-soft/LCUI/commit/3790fbd))
* **gui:** remove the mutex in the textview widget ([6340d60](https://github.com/lc-soft/LCUI/commit/6340d60))
* **gui:** save classes and status with strlist ([f611936](https://github.com/lc-soft/LCUI/commit/f611936))
* **gui:** update the sort method of the widget list ([e9dbcea](https://github.com/lc-soft/LCUI/commit/e9dbcea))
* **gui:** use LCUI_STYPE_INT instead of LCUI_STYPE_VALUE ([23090eb](https://github.com/lc-soft/LCUI/commit/23090eb))
* **renderer:** improve the selection of the rendering target ([1df08b6](https://github.com/lc-soft/LCUI/commit/1df08b6))



# [1.0.0-beta.4](https://github.com/lc-soft/LCUI/compare/v1.0.0-beta.3...v1.0.0-beta.4) (2018-11-19)

### Bug Fixes

* **dirent:** LCUI_ReadDirW() UNINITIALIZED READ ([dd610ad](https://github.com/lc-soft/LCUI/commit/dd610ad))
* **display:** the renderer pauses when resizing the window ([#164](https://github.com/lc-soft/LCUI/issues/164)) ([8ad667a](https://github.com/lc-soft/LCUI/commit/8ad667a))
* **display:** too many duplicate dirty rectangles in one frame ([b5d9040](https://github.com/lc-soft/LCUI/commit/b5d9040))
* **font:** failed to set default font after loading font file ([9b4c005](https://github.com/lc-soft/LCUI/commit/9b4c005))
* **font:** font number is not reset after module reinitialization ([d75b4ca](https://github.com/lc-soft/LCUI/commit/d75b4ca))
* **gui:** layout should be updated after widget display role is changed ([96c1cef](https://github.com/lc-soft/LCUI/commit/96c1cef))
* **gui:** should clear trash widget when module destroyed ([1ad6be1](https://github.com/lc-soft/LCUI/commit/1ad6be1))
* **gui:** the block element default width should be 100% ([adcb9e7](https://github.com/lc-soft/LCUI/commit/adcb9e7))
* **gui:** the node should unlinked before destroying widget ([3249490](https://github.com/lc-soft/LCUI/commit/3249490))
* **gui:** the unlink event is triggered repeatedly ([89faa5d](https://github.com/lc-soft/LCUI/commit/89faa5d))
* **gui:** widget background image is not rendered ([e76f3f9](https://github.com/lc-soft/LCUI/commit/e76f3f9))
* **gui:** Widget_AutoSize() should not change the static width or height ([8bee9c2](https://github.com/lc-soft/LCUI/commit/8bee9c2))
* **ime:** non-qwerty keyboard layouts broken ([#147](https://github.com/lc-soft/LCUI/issues/147)) ([4b1f050](https://github.com/lc-soft/LCUI/commit/4b1f050))
* **layout:** "left: auto" is calculated as "left: 0" ([f4990da](https://github.com/lc-soft/LCUI/commit/f4990da))
* **platform:** linux framebuffer driver has not been destroyed ([#157](https://github.com/lc-soft/LCUI/issues/157)) ([87f79f0](https://github.com/lc-soft/LCUI/commit/87f79f0))
* **renderer:** problem when widget has transparency ([#160](https://github.com/lc-soft/LCUI/issues/160)) ([d13b554](https://github.com/lc-soft/LCUI/commit/d13b554))
* **renderer:** transparent widgets are rendered incorrectly ([6668165](https://github.com/lc-soft/LCUI/commit/6668165))
* **textlayer:** chinese text line break error ([1e5a262](https://github.com/lc-soft/LCUI/commit/1e5a262))
* **textlayer:** isalpha() assertion failed ([cd39a46](https://github.com/lc-soft/LCUI/commit/cd39a46))
* **textview:** no effect after changing the "content" property ([d1ab50d](https://github.com/lc-soft/LCUI/commit/d1ab50d))
* **textview:** size not updated after setting text ([31e0582](https://github.com/lc-soft/LCUI/commit/31e0582))
* **util:** incorrect value of LCUI_DirEntry::name ([367febe](https://github.com/lc-soft/LCUI/commit/367febe))
* **worker:** worker thread memory access violation ([5a7e2c0](https://github.com/lc-soft/LCUI/commit/5a7e2c0))
* linker errors when jpeg and png are not supported ([2bbabe1](https://github.com/lc-soft/LCUI/commit/2bbabe1))
* PACKAGE_VERSION is not defined ([70660de](https://github.com/lc-soft/LCUI/commit/70660de))
* the mouse cursor should be hidden when the window system exists ([acf9454](https://github.com/lc-soft/LCUI/commit/acf9454))
* the return value of LCUI_GetAppId() is incorrect ([46ec607](https://github.com/lc-soft/LCUI/commit/46ec607))
* "linux/input.h" file not found on FreeBSD ([7d95f3f](https://github.com/lc-soft/LCUI/commit/7d95f3f))
* undefined ENODATA on FreeBSD ([2cd0b27](https://github.com/lc-soft/LCUI/commit/2cd0b27))
* unresolved external symbol _Graph_IsValid ([1328a2d](https://github.com/lc-soft/LCUI/commit/1328a2d))

### Features

* **builder:** `<resource>` support load xml file ([d5e162c](https://github.com/lc-soft/LCUI/commit/d5e162c))
* **display:** auto disable window maximize button ([#164](https://github.com/lc-soft/LCUI/issues/164)) ([82fad1b](https://github.com/lc-soft/LCUI/commit/82fad1b))
* **event:** add ctrl_key and shift_key to LCUI_KeyboardEvent ([6f5f17c](https://github.com/lc-soft/LCUI/commit/6f5f17c))
* **gui:** add Widget_SetOpacity() ([e6ad163](https://github.com/lc-soft/LCUI/commit/e6ad163))
* **gui:** add Widget_SetVisibility() ([145d9ee](https://github.com/lc-soft/LCUI/commit/145d9ee))
* **gui:** add Widget_SetVisible() and Widget_SetHidden() ([2f581b8](https://github.com/lc-soft/LCUI/commit/2f581b8))
* **gui:** LCUIWidget_ClearTrash() will return count ([6262f20](https://github.com/lc-soft/LCUI/commit/6262f20))
* **linux:** add mouse wheel event handing for x11 ([#54](https://github.com/lc-soft/LCUI/issues/54)) ([1061592](https://github.com/lc-soft/LCUI/commit/1061592))
* **linux:** use fontconfig to locate fonts ([2932246](https://github.com/lc-soft/LCUI/commit/2932246))
* **textview:** refresh all textview after loading new font ([3fcfa99](https://github.com/lc-soft/LCUI/commit/3fcfa99))
* **util:** add OpenUri() ([ec20c99](https://github.com/lc-soft/LCUI/commit/ec20c99))
* make LCUI_MAX_FRAMES_PER_SEC public ([6fa2995](https://github.com/lc-soft/LCUI/commit/6fa2995))
* add LCUI_MAX_FRAME_MSEC macro ([7abc901](https://github.com/lc-soft/LCUI/commit/7abc901))

### Performance Improvements

* **charset:** improve utf-8 and unicode string convert ([1efd856](https://github.com/lc-soft/LCUI/commit/1efd856))
* **graph:** improve image scaling quality (issue [#39](https://github.com/lc-soft/LCUI/issues/39)) ([bbfb9af](https://github.com/lc-soft/LCUI/commit/bbfb9af))

## 1.0.0 Beta 3 (2018-05-08)

### Bug Fixes

* **font:** segmentation fault in DeleteFont() ([5465c6b](https://github.com/lc-soft/LCUI/commit/5465c6b))
* **timer:** timer thread may quit after it is created ([0b01f88](https://github.com/lc-soft/LCUI/commit/0b01f88))
* **util:** 'struct dirent' has no member named 'd_reclen' ([#141](https://github.com/lc-soft/LCUI/issues/141)) ([0416c42](https://github.com/lc-soft/LCUI/commit/0416c42))

### Features

* **platform:** add linux framebuffer driver ([6015838](https://github.com/lc-soft/LCUI/commit/6015838))
* **platform:** add linux keyboard driver ([ad3348e](https://github.com/lc-soft/LCUI/commit/ad3348e))
* **platform:** add linux mouse driver ([0ff7b70](https://github.com/lc-soft/LCUI/commit/0ff7b70))

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

