
/** 初始化GUI部件绘制器 */
void LCUIWidgetPainter_Init(void);

/** 销毁GUI部件绘制器 */
void LCUIWidgetPainter_Destroy(void);

/* 设置部件内无效区域，该无效区域将被重绘 */
LCUI_API int Widget_InvalidateArea( LCUI_Widget *widget, LCUI_Rect *area );

/** 获取部件中的无效区域 */
LCUI_API int Widget_GetInvalidArea( LCUI_Widget *widget, LCUI_Rect *area );

/** 使部件中的一块区域有效化 */
LCUI_API void Widget_ValidateArea( LCUI_Widget *widget, LCUI_Rect *area );

/** 将部件的区域推送至屏幕 */
LCUI_API int Widget_PushAreaToScreen( LCUI_Widget *widget, LCUI_Rect *area );

/** 
 * 执行重绘部件前的一些任务
 * @param[in] widget 需要重绘的部件
 * @param[out] area 需要进行重绘的区域
 * @returns 正常返回TRUE，没有无效区域则返回FALSE
 */
LCUI_API LCUI_BOOL Widget_BeginPaint( LCUI_Widget *widget, LCUI_Rect *area );

/** 执行重绘部件后的一些任务 */
LCUI_API void Widget_EndPaint( LCUI_Widget *widget, LCUI_Rect *area );

/** 更新各个部件的无效区域中的内容 */
LCUI_API int LCUIWidget_ProcInvalidArea(void);
