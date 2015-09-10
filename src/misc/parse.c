#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/parse.h>

/** 从字符串中解析出数字，支持的单位：点(pt)、像素(px)、百分比(%) */
LCUI_BOOL ParseNumber( LCUI_StyleVar *var, const char *str )
{
	int n = 0;
	char num_str[32];
	const char *p = str;
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
			num_str[n++] = *p;
		}
		++p;
	}
	if( n == 0 ) {
		return FALSE;
	}
	num_str[n] = 0;
	switch( *p ) {
	case 'P':
	case 'p':
		if( p[1] == 'x' || p[1] == 'X' ) {
			var->type = SVT_PX;
			sscanf( num_str, "%d", &var->px );
		} else if( p[1] == 't' || p[1] == 'T' ) {
			var->type = SVT_PT;
			sscanf( num_str, "%d", &var->pt );
		} else {
			var->type = SVT_NONE;
		}
		break;
	case '%':
		if( 1 != sscanf(num_str, "%lf", &var->scale) ) {
			return FALSE;
		}
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

/** 从字符串中解析出色彩值，支持格式：#fff、#ffffff, rgba(R,G,B,A)、rgb(R,G,B) */
LCUI_BOOL ParseColor( LCUI_StyleVar *var, const char *str )
{
	double a;
	int len = 0, status = 0, r, g, b;
	const char *p;
	
	for( p=str; *p; ++p, ++len ) {
		switch( *p ) {
		case '#':
			len == 0 ? status = 3 : 0;
			break;
		case 'r':
			status == 0 ? status = 1 : 0;
			break;
		case 'g':
			status == 1 ? status <<= 1 : 0;
			break;
		case 'b':
			status == 2 ? status <<= 1 : 0;
			break;
		case 'a':
			status == 4 ? status <<= 1 : 0;
			break;
		default: 
			if( status < 3 ) {
				return FALSE;
			}
		}
	}
	switch( status ) {
	case 3:
		status = 0;
		if( len == 4 ) {
			status = sscanf( str, "#%1X%1X%1X", &r, &g, &b );
			r *= 255/0xf; g *= 255/0xf; b *= 255/0xf;
		} else if( len == 7 ) {
			status = sscanf( str, "#%2X%2X%2X", &r, &g, &b );
		}
		break;
	case 4:
		status = sscanf( str, "rgb(%d,%d,%d)", &r, &g, &b );
		break;
	case 8: 
		status = sscanf( str, "rgba(%d,%d,%d,%lf)", &r, &g, &b, &a );
	default:break;
	}
	if( status >= 3 ) {
		var->type = SVT_COLOR;
		if( status == 4 ) {
			var->color.a = (uchar_t)(255.0*a);
		} else {
			var->color.a = 255;
		}
		var->color.r = r;
		var->color.g = g;
		var->color.b = b;
		return TRUE;
	}
	return FALSE;
}
