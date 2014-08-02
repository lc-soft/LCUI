#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/parse.h>

/** 从字符串中解析出数字，支持的单位：点(pt)、像素(px)、百分比(%) */
LCUI_BOOL ParseNumer( StyleVar *var, const char *str )
{
	const char *p = str;
	char num[32], n = 0;
	LCUI_BOOL hasSign = FALSE, hasPoint = FALSE;

	if( str == NULL ) {
		return FALSE;
	}
	/* 先取出数值 */
	while(1) {
		if( *p >= '0' && *p <= '9' );
		else if( !hasSign && (*p == '-' || *p == '+') ) {
			hasSign = TRUE;
		}
		else if( !hasPoint && *p == '.' ) {
			hasPoint = TRUE;
		} else {
			break;
		}
		if( n < 30 ) {
			num[n++] = *p;
		}
		++p;
	}
	if( n == 0 ) {
		return FALSE;
	}
	num[31] = 0;
	switch( *p ) {
	case 'P':
	case 'p':
		if( p[1] == 'x' || p[1] == 'X' ) {
			var->type = SVT_PX;
			sscanf( num, "%d", &var->px );
		} else if( p[1] == 't' || p[1] == 'T' ) {
			var->type = SVT_PT;
			sscanf( num, "%d", &var->pt );
		} else {
			var->type = SVT_NONE;
		}
		break;
	case '%':
		sscanf( num, "%lf", &var->scale );
		var->scale /= 100.0;
		var->type = SVT_SCALE; 
		break;
	case 0: 
	default: 
		var->type = SVT_NONE; 
		break;
	}
	return TRUE;
}

/** 从字符串中解析出色彩值，支持格式：#fff、#ffffff, rgba(R,G,B,A) */
LCUI_BOOL ParseColor( StyleVar *var, const char *str )
{
	return FALSE;
}
