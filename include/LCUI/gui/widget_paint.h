
/** 初始化GUI部件绘制器 */
void LCUIWidgetPainter_Init(void);

/** 销毁GUI部件绘制器 */
void LCUIWidgetPainter_Destroy(void);

/* 在指定部件的内部区域内设定需要刷新的区域 */
LCUI_API int Widget_InvalidateArea( LCUI_Widget *widget, LCUI_Rect rect );

/** 获取部件中的无效区域 */
LCUI_API int Widget_GetInvalidArea( LCUI_Widget *widget, LCUI_Rect *area );

/** 使部件中的一块区域有效化 */
LCUI_API void Widget_ValidateArea( LCUI_Widget *widget, LCUI_Rect area );

/** 更新各个部件的无效区域中的内容 */
LCUI_API int LCUIWidget_ProcInvalidArea(void);
