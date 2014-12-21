// IME: Input Method Editor/Engine
//#define NEED_IME
#ifdef NEED_IME
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/widget.h>
#include <LCUI/ime.h>

typedef struct LCUIIME_Info_ {
	int id;
	LCUI_String name;
	LCUIIME_Func func;
} LCUIIME_Info;

// 指示输入法列表是否初始化
LCUI_BOOL imelist_init = FALSE;

// 输入法列表
LCUI_Queue imelist;

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
LCUI_API int LCUIIME_Register( const char *ime_name, LCUIIME_Func *ime_func )
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
	if( !ptr_data ) {
		return -3;
	}
	LCUIString_Init( &ptr_data->name );
	_LCUIString_Copy( &ptr_data->name, ime_name );
	ime_id = ime_id + 1;
	ptr_data->id = ime_id;
	memcpy( &ptr_data->func, ime_func, sizeof(LCUIIME_Func) );
	Queue_AddPointer( &imelist, ptr_data );
	return ime_id;
}

/* 选定输入法 */
LCUI_API LCUI_BOOL LCUIIME_Select( int ime_id )
{
	LCUIIME_Info *ptr_data;
	ptr_data = LCUIIME_Find( ime_id );
	if( ptr_data == NULL ) {
		return FALSE;
	}
	current_ime = ptr_data;
	return TRUE;
}

LCUI_API LCUI_BOOL LCUIIME_SelectByName( const char *name )
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
static LCUI_BOOL LCUIIME_Open( LCUIIME_Info *ime )
{
	if( ime == NULL ) {
		return FALSE;
	}
	if( ime->func.open == NULL ) {
		return FALSE;
	}
	return ime->func.open();
}

/* 关闭输入法 */
static LCUI_BOOL LCUIIME_Close( LCUIIME_Info *ime )
{
	if( ime == NULL ) {
		return FALSE;
	}
	if( ime->func.close == NULL ) {
		return FALSE;
	}
	return ime->func.close();
}

/* 切换至下一个输入法 */
LCUI_API void LCUIIME_Switch(void)
{
	int i=0, n;
	LCUIIME_Info *ime_ptr;

	n = Queue_GetTotal( &imelist );
	if( i>=n ) {
		i = 0;
	}
	ime_ptr = Queue_Get( &imelist, i );
	if( ime_ptr == NULL ) {
		return;
	}
	if(ime_ptr != current_ime) {
		LCUIIME_Close( current_ime );
		current_ime = ime_ptr;
		LCUIIME_Open( current_ime );
	}
	++i;
}

/* 销毁输入法信息 */
static void LCUIIME_DestroyInfo( void *arg )
{
	LCUIIME_Info *p;
	p = (LCUIIME_Info*)arg;
	LCUIString_Free( &p->name );
}

static void LCUIIME_ToText( const LCUI_KeyboardEvent *event  )
{
	char ch;

	switch(event->key_code) {
#ifdef LCUI_BUILD_IN_WIN32
	case 189: ch = '-'; break;
	case 187: ch = '='; break;
	case 188: ch = ','; break;
	case 190: ch = '.'; break;
	case 191: ch = '/'; break;
	case 222: ch = '\''; break;
	case 186: ch = ';'; break;
	case 220: ch = '\\'; break;
	case 221: ch = ']'; break;
	case 219: ch = '['; break;
#endif
	default:ch = event->key_code;break;
	}

	DEBUG_MSG("key code: %d\n", event->key_code);
	/* 如果没开启大写锁定，则将字母转换成小写 */
	if( !enable_capitals_lock ) {
		if( ch >= 'A' && ch <= 'Z' ) {
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
	DEBUG_MSG("ch = %c\n", ch);
	current_ime->func.totext( ch );
}

/* 检测输入法是否要处理按键事件 */
LCUI_API LCUI_BOOL LCUIIME_ProcessKey( const LCUI_KeyboardEvent *event )
{
	int key_state;
	/* 根据事件类型判定按键状态 */
	if( event->type == LCUI_KEYUP ) {
		key_state = LCUIKEYSTATE_RELEASE;
	} else {
		key_state = LCUIKEYSTATE_PRESSED;
	}
	/* 如果按下的是shift键，但没释放，则直接退出 */
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
	if( current_ime->func.prockey( event->key_code, key_state ) ) {
		LCUIIME_ToText( event );
		return TRUE;
	}
	return FALSE;
}

/* 提交输入法输入的内容至目标 */
LCUI_API int LCUIIME_Commit( const wchar_t *str )
{
	LCUI_Widget widget;
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
LCUI_API int LCUIIME_SetTarget( LCUI_Widget widget )
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

static int LCUI_DefaultIMERegister(void);

/* 初始化LCUI输入法模块 */
LCUI_API void LCUIModule_IME_Init(void)
{
	int ime_id;
	Queue_Init( &imelist, sizeof(LCUIIME_Info), LCUIIME_DestroyInfo );
	imelist_init = TRUE;
	ime_id = LCUI_DefaultIMERegister();
	LCUIIME_Select( ime_id );
}

/* 停用LCUI输入法模块 */
LCUI_API void LCUIModule_IME_End(void)
{
	imelist_init = FALSE;
}


/*-------------------------- 默认的输入法 ------------------------------*/

// 目标部件
static LCUI_Widget target_widget = NULL;

/**
判断按键是否需要处理，也可以用于设置当前的输入法，比如：
按shift键，切换输入法的中英文输入，按PageUP和PageDown切换词组页
按backspace键，如果输入的内容为空，则return FALSE;让LCUI将该按键事件发给部件
的相应回调函数处理；否则，return TRUE;并从输入的内容中删除一个字符
**/
static LCUI_BOOL IME_ProcessKey( int key, int key_state )
{
#ifdef LCUI_BUILD_IN_LINUX
	/* *
	 * LCUI的linux键盘驱动获取的是输入的字符的ASCII码，而不是实际键值，
	 * 因此，直接判断是否为可显字符即可
	 * */
	if( key >= '!' && key <= '~') {
		return TRUE;
	}
#elif defined LCUI_BUILD_IN_WIN32
	if( key_state != LCUIKEYSTATE_PRESSED ) {
		return FALSE;
	}
#endif
	switch(key) {
	case LCUIKEY_ENTER:
	case LCUIKEY_SPACE:
#ifdef LCUI_BUILD_IN_WIN32
	case LCUIKEY_0:
	case LCUIKEY_1:
	case LCUIKEY_2:
	case LCUIKEY_3:
	case LCUIKEY_4:
	case LCUIKEY_5:
	case LCUIKEY_6:
	case LCUIKEY_7:
	case LCUIKEY_8:
	case LCUIKEY_9:
	case LCUIKEY_A:
	case LCUIKEY_B:
	case LCUIKEY_C:
	case LCUIKEY_D:
	case LCUIKEY_E:
	case LCUIKEY_F:
	case LCUIKEY_G:
	case LCUIKEY_H:
	case LCUIKEY_I:
	case LCUIKEY_J:
	case LCUIKEY_K:
	case LCUIKEY_L:
	case LCUIKEY_M:
	case LCUIKEY_N:
	case LCUIKEY_O:
	case LCUIKEY_P:
	case LCUIKEY_Q:
	case LCUIKEY_R:
	case LCUIKEY_S:
	case LCUIKEY_T:
	case LCUIKEY_U:
	case LCUIKEY_V:
	case LCUIKEY_W:
	case LCUIKEY_X:
	case LCUIKEY_Y:
	case LCUIKEY_Z:
	case 189:
	case 187:
	case 188:
	case 190:
	case 191:
	case 222:
	case 186:
	case 220:
	case 221:
	case 219:
#endif
	return TRUE;
	default:break;
	}
	return FALSE;
}

static void IME_SetTarget( LCUI_Widget widget )
{
	target_widget = widget;
}

static LCUI_Widget* IME_GetTarget( void )
{
	return target_widget;
}

static void IME_ToText( char ch )
{
	wchar_t text[2];
	
#ifdef LCUI_BUILD_IN_WIN32
	/* 回车符转换行符 */
	if( ch == '\r' ) {
		ch = '\n';
	}
#endif
	text[0] = ch;
	text[1] = '\0';
	DEBUG_MSG("%S, %d\n", text, ch);
	LCUIIME_Commit( text ); // 直接提交该字符
}

/**
 输入法被打开时的处理
 可以在输入法被打开时，初始化相关数据，链接至词库什么的
 **/
static LCUI_BOOL IME_Open(void)
{
	return TRUE;
}


/* 输入法被关闭时的处理 */
static LCUI_BOOL IME_Close(void)
{
	return TRUE;
}

/* 注册LCUI默认的输入法 */
static int LCUI_DefaultIMERegister(void)
{
	LCUIIME_Func ime_func;
	ime_func.gettarget = IME_GetTarget;
	ime_func.settarget = IME_SetTarget;
	ime_func.prockey = IME_ProcessKey;
	ime_func.totext = IME_ToText;
	ime_func.close = IME_Close;
	ime_func.open = IME_Open;
	return LCUIIME_Register( "LCUI Input Method", &ime_func );
}
/*------------------------------ END ----------------------------------*/

#endif
