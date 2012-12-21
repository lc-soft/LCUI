
#define ENCODEING_TYPE_UTF8	0
#define ENCODEING_TYPE_GB2312	1

int Get_EncodingType();
/* 获取字符编码类型 */

int Using_GB2312();
/* 
 * 说明：如果你的系统只能使用GB2312编码，不能使用UTF-8编码，可以使用这
 * 个函数进行设置，让相关函数正常转换字符编码 
 * */

int Char_To_Wchar_T(char *in_text, wchar_t **unicode_text);
/*
 * 功能：将char型字符串转换成wchar_t字符串
 * 参数说明：
 * in_text      ：传入的char型字符串
 * unicode_text ：输出的wchar_t型字符串
 * 返回值：正常则wchar_t型字符串的长度，否则返回-1
 * */
