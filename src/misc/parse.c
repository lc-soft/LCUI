#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/parse.h>

/** 从字符串中解析出数值，包括px、%、dp等单位 */
LCUI_BOOL ParseNumber( LCUI_Style *s, const char *str )
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
	case 'd':
	case 'D':
		if( p[1] == 'p' || p[1] == 'P' ) {
			s->type = SVT_DP;
			sscanf( num_str, "%d", &s->dp );
		}else {
			s->type = SVT_NONE;
		}
		break;
	case 'P':
	case 'p':
		if( p[1] == 'x' || p[1] == 'X' ) {
			s->type = SVT_PX;
			sscanf( num_str, "%d", &s->px );
		} else if( p[1] == 't' || p[1] == 'T' ) {
			s->type = SVT_PT;
			sscanf( num_str, "%d", &s->pt );
		} else {
			s->type = SVT_NONE;
		}
		break;
	case '%':
		if( 1 != sscanf(num_str, "%f", &s->scale) ) {
			return FALSE;
		}
		s->scale /= 100.0;
		s->type = SVT_SCALE;
		break;
	case 0:
		if( 1 == sscanf(num_str, "%d", &s->px) ) {
			s->type = SVT_PX;
			break;
		}
	default:
		s->type = SVT_NONE;
		s->is_valid = FALSE;
		return FALSE;
	}
	s->is_valid = TRUE;
	s->is_changed = TRUE;
	return TRUE;
}

/** 从字符串中解析出色彩值，支持格式：#fff、#ffffff, rgba(R,G,B,A)、rgb(R,G,B) */
LCUI_BOOL ParseColor( LCUI_Style *var, const char *str )
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
		var->is_valid = TRUE;
		var->is_changed = TRUE;
		return TRUE;
	}
	return FALSE;
}
