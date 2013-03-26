// IME: Input Method Editor/Engine
#define NEED_IME
#ifdef NEED_IME
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_INPUT_H
#include LC_WIDGET_H

typedef struct LCUIIME_Func_ {
	LCUI_BOOL (*prockey)(int);
	void (*settarget)(LCUI_Widget*);
	LCUI_Widget *(*gettarget)(void);
	void (*totext)(char);
} LCUIIME_Func;

typedef struct LCUIIME_Info_ {
	int id;
	LCUI_String name;
	LCUIIME_Func func;
} LCUIIME_Info;

// 指示输入法列表是否初始化
LCUI_BOOL imelist_init = FALSE;

// 输入法列表
LCUI_Queue imelist;

// 输入缓冲区，用于保存输入法输入的文本
static wchar_t text_input_buff[256];

// 当前选择的输入法
static LCUIIME_Info* current_ime = NULL;

// 指示是否开启了大写锁定
static LCUI_BOOL enable_capitals_lock = FALSE;

/* 通过ID查找输入法信息 */
static LCUIIME_Info* LCUIIME_Find( int ime_id )
{
	int n, i;
	LCUIIME_Info *ptr;

	n = Queue_GetTotal( &imelist );
	for( i=0; i<n; ++i ) {
		ptr = Queue_Get( &imelist, i );
		if( !ptr || ptr->id != ime_id ) {
			continue;
		}
		return ptr;
	}
	return NULL;
}

/* 通过名字查找输入法信息 */
static LCUIIME_Info* LCUIIME_FindByName( const char *name )
{
	int n, i;
	LCUIIME_Info *ptr;

	n = Queue_GetTotal( &imelist );
	for( i=0; i<n; ++i ) {
		ptr = Queue_Get( &imelist, i );
		if( !ptr ) {
			continue;
		}
		if( _LCUIString_Cmp( &ptr->name, name ) == 0 ) {
			return ptr;
		}
	}
	return NULL;
}

/* 注册一个输入法 */
LCUI_EXPORT(int)
LCUIIME_Register( const char *ime_name, LCUIIME_Func *ime_func )
{
	static int ime_id = 0; // 输入法的ID
	LCUIIME_Info *ptr_data;

	if( !imelist_init ) {
		return -1;
	}
	if( LCUIIME_FindByName( ime_name ) != NULL ) {
		return -2;
	}

	ptr_data = (LCUIIME_Info*)malloc( sizeof(LCUIIME_Info) );
	_LCUIString_Copy( &ptr_data->name, ime_name );
	ime_id = ime_id + 1;
	ptr_data->id = ime_id;
	memcpy( &ptr_data->func, ime_func, sizeof(LCUIIME_Func) );
	Queue_Add( &imelist, ptr_data );
	return ime_id;
}

/* 选定输入法 */
LCUI_EXPORT(LCUI_BOOL)
LCUIIME_Select( int ime_id )
{
	LCUIIME_Info *ptr_data;
	ptr_data = LCUIIME_Find( ime_id );
	if( ptr_data == NULL ) {
		return FALSE;
	}
	current_ime = ptr_data;
	return TRUE;
}

LCUI_EXPORT(LCUI_BOOL)
LCUIIME_SelectByName( const char *name )
{
	LCUIIME_Info *ptr_data;
	ptr_data = LCUIIME_FindByName( name );
	if( ptr_data == NULL ) {
		return FALSE;
	}
	current_ime = ptr_data;
	return TRUE;
}

/* 打开输入法 */
LCUIIME_Open(void)
{

}

/* 关闭输入法 */
LCUIIME_Close(void)
{

}

/* 切换至下一个输入法 */
LCUI_EXPORT(void)
LCUIIME_Switch(void)
{

}

static void LCUIIME_DestroyInfo( void *arg )
{
	LCUIIME_Info *p;
	p = (LCUIIME_Info*)arg;
	LCUIString_Free( &p->name );
}

static void LCUIIME_ToText( const LCUI_KeyboardEvent *event  )
{
	char ch;
	
	if( event->key_code >= 'A' && event->key_code <= 'Z' ) {
		/* 如果开启了大写锁定 */
		if( enable_capitals_lock ) {
			ch = event->key_code;
		} else {
			ch = event->key_code + 32;
		}
	}
	/* 如果shift键处于按下状态 */
	if( LCUIKey_IsHit(LCUIKEY_SHIFT) ) {
		if(ch >='a' && ch <= 'z') {
			ch = ch - 32;
		} else if(ch >='A' && ch <= 'Z') {
			ch = ch + 32;
		} else {
			switch(ch) {
			case '1': ch = '!'; break;
			case '2': ch = '@'; break;
			case '3': ch = '#'; break;
			case '4': ch = '$'; break;
			case '5': ch = '%'; break;
			case '6': ch = '^'; break;
			case '7': ch = '&'; break;
			case '8': ch = '*'; break;
			case '9': ch = '('; break;
			case '0': ch = ')'; break;
			case '`': ch = '~'; break;
			case '-': ch = '_'; break;
			case '=': ch = '+'; break;
			case '[': ch = '{'; break;
			case ']': ch = '}'; break;
			case '\\': ch = '|'; break;
			case ';': ch = ':'; break;
			case '\'': ch = '"'; break;
			case ',': ch = '<'; break;
			case '.': ch = '>'; break;
			case '/': ch = '?'; break;
			}
		}
	}
	current_ime->func.totext( ch );
}

/* 检测输入法是否要处理按键事件 */
LCUI_EXPORT(LCUI_BOOL)
LCUIIME_ProcessKey( const LCUI_KeyboardEvent *event )
{
	/* 如果按下下的是shift键，但没释放，则直接退出 */
	if( event->key_code == LCUIKEY_SHIFT
	 && event->type == LCUI_KEYDOWN ) {
		return FALSE;
	}
	/* 如果是caps lock按键被释放 */
	if( event->key_code == LCUIKEY_CAPITAL
	&& event->type == LCUI_KEYUP ) {
		enable_capitals_lock = !enable_capitals_lock;
		return FALSE;
	}
	if( current_ime == NULL ) {
		return FALSE;
	}
	if( current_ime->func.prockey == NULL ) {
		return FALSE;
	}
	/* 如果输入法要处理该键，则调用LCUIIME_ToText函数 */
	if( current_ime->func.prockey( event->key_code ) ) {
		LCUIIME_ToText( event );
		return TRUE;
	}
	return FALSE;
}

/* 提交输入法输入的内容至目标 */
LCUI_EXPORT(int)
LCUIIME_Commit( const wchar_t *str )
{
	LCUI_Widget *widget;
	LCUI_WidgetEvent event;

	if( current_ime == NULL ) {
		return -1;
	}
	if( current_ime->func.gettarget == NULL ) {
		return -2;
	}
	widget = current_ime->func.gettarget();
	if(! widget ) {
		return -3;
	}
	/* 设置事件 */
	event.type = EVENT_INPUT;
	wcscpy( event.input.text, str );
	/* 派发事件 */
	return Widget_DispatchEvent( widget, &event );
}

/* 设置输入法的目标 */
LCUI_EXPORT(int)
LCUIIME_SetTarget( LCUI_Widget *widget )
{
	if( current_ime == NULL ) {
		return -1;
	}
	if( current_ime->func.settarget == NULL ) {
		return -2;
	}
	current_ime->func.settarget( widget );
	return 0;
}


LCUI_EXPORT(void)
LCUIModule_IME_Init(void)
{
	Queue_Init( &imelist, sizeof(LCUIIME_Info), LCUIIME_DestroyInfo );
	
	imelist_init = TRUE;
}

LCUI_EXPORT(void)
LCUIModule_IME_End(void)
{
	imelist_init = FALSE;
}


/*-------------------------- 默认的输入法 ------------------------------*/

// 目标部件
static LCUI_Widget *target_widget = NULL;

/**
判断按键是否需要处理，也可以用于设置当前的输入法，比如：
按shift键，切换输入法的中英文输入，按PageUP和PageDown切换词组页
按backspace键，如果输入的内容为空，则return FALSE;让LCUI将该按键事件发给部件
的相应回调函数处理；否则，return TRUE;并从输入的内容中删除一个字符
**/
static LCUI_BOOL
IME_ProcessKey( int key )
{
	switch(key) {
	case LCUIKEY_SHIFT:break;
	return TRUE;
	}
	return FALSE;
}

static void
IME_SetTarget( LCUI_Widget *widget )
{
	target_widget = widget;
}

static LCUI_Widget*
IME_GetTarget( void )
{
	return target_widget;
}

static void
IME_ToText( char ch )
{
	wchar_t text[2];
	
	text[0] = ch;
	text[1] = '\0';
	LCUIIME_Commit( text ); // 直接提交该字符
}
/*------------------------------ END ----------------------------------*/

#endif