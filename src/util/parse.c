/* parse.c -- Parse data from string
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/util/parse.h>
#include <LCUI/font/fontlibrary.h>

LCUI_BOOL ParseNumber( LCUI_Style s, const char *str )
{
	int n = 0;
	const char *p;
	char num_str[32];
	LCUI_BOOL has_point = FALSE;

	if( str == NULL ) {
		return FALSE;
	}
	/* 先取出数值 */
	for( p = str; *p && n < 30; ++p ) {
		if( *p >= '0' && *p <= '9' );
		else if( *p == '-' || *p == '+' ) {
			if( n > 0 ) {
				n = 0;
				break;
			}
		} else if( *p == '.' ) {
			if( has_point ) {
				n = 0;
				break;
			}
			has_point = TRUE;
		} else {
			break;
		}
		num_str[n++] = *p;
	}
	if( n == 0 ) {
		return FALSE;
	}
	num_str[n] = 0;
	switch( *p ) {
	case 'd':
	case 'D':
		s->type = SVT_NONE;
		if( !p[1] ) {
			break;
		}
		if( p[1] == 'p' || p[1] == 'P' ) {
			s->type = SVT_DIP;
			sscanf( num_str, "%f", &s->dip );
			break;
		}
		if( p[1] == 'i' || p[1] == 'I' ) {
			if( p[2] == 'p' || p[2] == 'P' ) {
				s->type = SVT_DIP;
				sscanf( num_str, "%f", &s->dip );
				break;
			}
		}
		break;
	case 's':
	case 'S':
		if( p[1] == 'p' || p[1] == 'P' ) {
			s->type = SVT_SP;
			sscanf( num_str, "%f", &s->sp );
		} else {
			s->type = SVT_NONE;
		}
		break;
	case 'P':
	case 'p':
		if( p[1] == 'x' || p[1] == 'X' ) {
			s->type = SVT_PX;
			sscanf( num_str, "%f", &s->px );
		} else if( p[1] == 't' || p[1] == 'T' ) {
			s->type = SVT_PT;
			sscanf( num_str, "%f", &s->pt );
		} else {
			s->type = SVT_NONE;
		}
		break;
	case '%':
		if( 1 != sscanf( num_str, "%f", &s->scale ) ) {
			return FALSE;
		}
		s->scale /= 100.0;
		s->type = SVT_SCALE;
		break;
	case 0:
		if( has_point && 1 == sscanf( num_str, "%f", &s->scale ) ) {
			s->type = SVT_SCALE;
			break;
		}
		if( 1 == sscanf( num_str, "%d", &s->val_int ) ) {
			s->type = SVT_VALUE;
			break;
		}
	default:
		s->type = SVT_NONE;
		s->is_valid = FALSE;
		return FALSE;
	}
	s->is_valid = TRUE;
	return TRUE;
}

LCUI_BOOL ParseRGBA( LCUI_Style var, const char *str )
{
	float data[4];
	char buf[16];
	const char *p;
	int i, buf_i;

	if( !strstr( str, "rgba(" ) ) {
		return FALSE;
	}
	for( p = str + 5, i = 0, buf_i = 0; *p && i < 4; ++p ) {
		if( *p == '.' || (*p >= '0' && *p <= '9') ) {
			if( buf_i < 15 ) {
				buf[buf_i++] = *p;
			}
			continue;
		}
		if( *p == ' ' ) {
			buf_i = 0;
			continue;
		}
		if( *p == ',' || *p == ')' ) {
			buf[buf_i] = 0;
			sscanf( buf, "%f", &data[i] );
			buf_i = 0;
			i += 1;
		}
	}
	if( *p ) {
		return FALSE;
	}
	var->type = SVT_COLOR;
	var->color.a = (uchar_t)(255.0 * data[3]);
	var->color.r = (uchar_t)data[0];
	var->color.g = (uchar_t)data[1];
	var->color.b = (uchar_t)data[2];
	var->is_valid = TRUE;
	return TRUE;
}

LCUI_BOOL ParseRGB( LCUI_Style var, const char *str )
{
	float data[3];
	char buf[16];
	const char *p;
	int i, buf_i;

	if( !strstr( str, "rgb(" ) ) {
		return FALSE;
	}
	for( p = str + 4, i = 0, buf_i = 0; *p && i < 3; ++p ) {
		if( *p == '.' || (*p >= '0' && *p <= '9') ) {
			if( buf_i < 15 ) {
				buf[buf_i++] = *p;
			}
			continue;
		}
		if( *p == ' ' ) {
			buf_i = 0;
			continue;
		}
		if( *p == ',' || *p == ')' ) {
			buf[buf_i] = 0;
			sscanf( buf, "%f", &data[i] );
			buf_i = 0;
			i += 1;
		}
	}
	if( *p ) {
		return FALSE;
	}
	var->type = SVT_COLOR;
	var->color.a = 255;
	var->color.r = (uchar_t)data[0];
	var->color.g = (uchar_t)data[1];
	var->color.b = (uchar_t)data[2];
	var->is_valid = TRUE;
	return TRUE;
}

LCUI_BOOL ParseColor( LCUI_Style var, const char *str )
{
	const char *p;
	int len = 0, status = 0, r, g, b;
	for( p = str; *p; ++p, ++len ) {
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
				break;
			}
		}
	}
	switch( status ) {
	case 3:
		status = 0;
		if( len == 4 ) {
			status = sscanf( str, "#%1X%1X%1X", &r, &g, &b );
			r *= 255 / 0xf; g *= 255 / 0xf; b *= 255 / 0xf;
		} else if( len == 7 ) {
			status = sscanf( str, "#%2X%2X%2X", &r, &g, &b );
		}
		break;
	case 4: return ParseRGB( var, str );
	case 8: return ParseRGBA( var, str );
	default:break;
	}
	if( status == 3 ) {
		var->type = SVT_COLOR;
		var->color.a = 255;
		var->color.r = r;
		var->color.g = g;
		var->color.b = b;
		var->is_valid = TRUE;
		return TRUE;
	}
	if( strcmp( "transparent", str ) == 0 ) {
		var->is_valid = TRUE;
		var->color.alpha = 0;
		var->color.red = 255;
		var->color.green = 255;
		var->color.blue = 255;
		var->type = SVT_COLOR;
		return TRUE;
	}
	return FALSE;
}

static LCUI_BOOL IsAbsolutePath( const char *path )
{
	if( path[0] == '/' ) {
		return TRUE;
	}
	if( isalpha( path[0] ) && path[1] == ':' && path[2] == '/' ) {
		return TRUE;
	}
	return FALSE;
}

LCUI_BOOL ParseUrl( LCUI_Style s, const char *str, const char *dirname )
{
	size_t n, dirname_len;
	const char *p, *head, *tail;

	p = str;
	tail = head = strstr( p, "url(" );
	if( !head ) {
		return FALSE;
	}
	while( p ) {
		tail = p;
		p = strstr( p + 1, ")" );
	}
	if( tail == head ) {
		return FALSE;
	}
	head += 4;
	if( *head == '"' ) {
		++head;
	}
	n = tail - head;
	s->type = SVT_STRING;
	if( dirname && !IsAbsolutePath( head ) ) {
		n += (dirname_len = strlen( dirname ));
		s->val_string = malloc( (n + 1) * sizeof( char ) );
		if( !s->val_string ) {
			return FALSE;
		}
		strcpy( s->val_string, dirname );
		if( s->val_string[dirname_len - 1] != '/' ) {
			s->val_string[dirname_len] = '/';
			dirname_len += 1;
		}
		strncpy( s->val_string + dirname_len,
			 head, n - dirname_len );
		s->val_string[n] = 0;
	} else {
		s->val_string = malloc( (n + 1) * sizeof( char ) );
		if( !s->val_string ) {
			return FALSE;
		}
		strncpy( s->val_string, head, n );
	}
	s->is_valid = TRUE;
	s->val_string[n] = 0;
	if( n > 0 && s->val_string[n - 1] == '"' ) {
		n -= 1;
		s->val_string[n] = 0;
	}
	return TRUE;
}

LCUI_BOOL ParseFontWeight( const char *str, int *weight )
{
	int value;
	if( strcmp( str, "normal" ) == 0 ) {
		*weight = FONT_WEIGHT_NORMAL;
		return TRUE;
	}
	if( strcmp( str, "bold" ) == 0 ) {
		*weight = FONT_WEIGHT_BOLD;
		return TRUE;
	}
	if( sscanf( str, "%d", &value ) != 1 ) {
		*weight = FONT_WEIGHT_NORMAL;
		return FALSE;
	}
	if( value < 100 ) {
		*weight = FONT_WEIGHT_THIN;
		return TRUE;
	}
	*weight = iround( value / 100.0 ) * 100;
	return TRUE;
}

LCUI_BOOL ParseFontStyle( const char *str, int *style )
{
	char value[64] = "";
	strtrim( value, str, NULL );
	if( strcmp( value, "normal" ) == 0 ) {
		*style = FONT_STYLE_NORMAL;
	} else if( strcmp( value, "italic" ) == 0 ) {
		*style = FONT_STYLE_ITALIC;
	} else if( strcmp( value, "oblique" ) == 0 ) {
		*style = FONT_STYLE_OBLIQUE;
	} else {
		return FALSE;
	}
	return TRUE;
}
