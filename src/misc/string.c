#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_FONT_H

void String_Init(LCUI_String *in)
/* 功能：初始化String结构体中的数据 */
{
	in->size = 0;
	in->string = NULL;
}

void Strcpy (LCUI_String * des, const char *src)
/* 功能：拷贝字符串至String结构体数据中 */
{
	if(des == NULL) {
		return;
	}
	if (src != NULL) {
		if (des->size != 0) {
			free (des->string);
		}
		des->size = sizeof (char) * (strlen (src) + 1);
		des->string = calloc (1, des->size);
		strcpy (des->string, src);
	} else {
		des->size = sizeof (char) * (2);
		des->string = calloc (1, des->size);
	}
}

int Strcmp(LCUI_String *str1, const char *str2)
/* 功能：对比str1与str2 */
{
	if (str1 != NULL && str1->size > 0 && str2 != NULL) 
		return strcmp(str1->string, str2); 
	else return -1;
}

int LCUI_Strcmp(LCUI_String *str1, LCUI_String *str2)
/* LCUI_String 字符串对比 */
{
	if( str1->size > 0 && str2->size > 0 ) {
		return strcmp(str1->string, str2->string);
	}
	return 0;
}

int LCUI_Strcpy(LCUI_String *str1, LCUI_String *str2)
/* LCUI_String 字符串拷贝 */
{
	if( str1->size <=0 || str2->size <= 0 ) {
		return -1;
	}
	if(str1->size > 0) {
		if( str2->size > str1->size ) { 
			free( str1->string );
			str1->string = (char*)calloc(str2->size+1, sizeof(char));
		}
	}
	strcpy( str1->string, str2->string );
	str1->size = str2->size;
	return 0;
}

void String_Free(LCUI_String *in) 
{
	if(in->size > 0) {
		free(in->string); 
	}
	in->string = NULL;
}

void WChar_T_Free(LCUI_WChar_T *ch) 
{
	FontBMP_Free(&ch->bitmap);
}

void WString_Free(LCUI_WString *str) 
{
	int i;
	if(str != NULL) {
		if(str->size > 0 && str->string != NULL) {
			for(i = 0; i < str->size; ++i) {
				WChar_T_Free(&str->string[i]); 
			}
			free(str->string);
			str->string = NULL;
		}
	}
}
