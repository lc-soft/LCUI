#ifndef __LCUI_IME_H__
#define __LCUI_IME_H__

LCUI_BEGIN_HEADER

typedef struct LCUIIME_Func_ {
	LCUI_BOOL (*prockey)(int);
	void (*settarget)(LCUI_Widget*);
	LCUI_Widget *(*gettarget)(void);
	void (*totext)(char);
	LCUI_BOOL (*open)(void);
	LCUI_BOOL (*close)(void);
} LCUIIME_Func;

/* 注册一个输入法 */
LCUI_API int
LCUIIME_Register( const char *ime_name, LCUIIME_Func *ime_func );

/* 选定输入法 */
LCUI_API LCUI_BOOL
LCUIIME_Select( int ime_id );

LCUI_API LCUI_BOOL
LCUIIME_SelectByName( const char *name );

/* 切换至下一个输入法 */
LCUI_API void
LCUIIME_Switch(void);

/* 检测输入法是否要处理按键事件 */
LCUI_API LCUI_BOOL
LCUIIME_ProcessKey( const LCUI_KeyboardEvent *event );

/* 提交输入法输入的内容至目标 */
LCUI_API int
LCUIIME_Commit( const wchar_t *str );

/* 设置输入法的目标 */
LCUI_API int
LCUIIME_SetTarget( LCUI_Widget *widget );

/* 初始化LCUI输入法模块 */
LCUI_API void
LCUIModule_IME_Init(void);

/* 停用LCUI输入法模块 */
LCUI_API void
LCUIModule_IME_End(void);

LCUI_END_HEADER

#endif