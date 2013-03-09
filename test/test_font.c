// 测试 LCUI 的字体处理功能
// 由于LCUI的版本变更，该程序的功能暂时无法完全实现
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_PICBOX_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_INPUT_H 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <locale.h>
#include <math.h>

static LCUI_Widget *window, *text; 
static char **filename = NULL;
static int total_files = 0, current = 0;

void get_filepath(char *filepath, char *out_path)
/* 功能：用于获取程序所在的文件目录 */
{
	int num;
	strcpy(out_path, filepath);
	for(num = strlen(filepath) - 1; num >= 0; --num) 
	{
		if(filepath[num] == '/') 
		{
			out_path[num+1] = 0;
			return;
		}
	}
	memset(out_path, 0, sizeof(char)*strlen(out_path)); 
}
 
int get_format(char *format, char *filename)
/* 功能：获取文件格式 */
{
	int i, j, len;
	len = strlen(filename);
	for(i=len-1; i>=0; --i)
	{
		if(filename[i] == '.')
		{
			for(j=0, ++i; i<len; ++i, ++j)
			{
				format[j] = filename[i];
			}
			format[j] = 0;
			return 0;
		}
	}
	strcpy(format, "");
	return -1;
} 

 
char **scan_fontfile(char *dir, int *file_num)
/* 功能：扫描字体文件，并获得文件列表 */
{
	int i, len, n; 
	char **filelist, format[256], path[1024];
	struct dirent **namelist;
	if(strlen(dir) == 0)
		n = scandir(".", &namelist, 0, alphasort);
	else	
	n = scandir(dir, &namelist, 0, alphasort);
	
	if (n < 0) {
		return 0; 
	} else {
		filelist = (char **)malloc(sizeof(char *)*n);
		for(i=0, *file_num=0; i<n; i++)
		{
			if(namelist[i]->d_type==8)/* 如果是文件 */ 
			{
				get_format(format, namelist[i]->d_name);
				if(strlen(format) > 2 
				&& (strcasecmp(format, "ttf") == 0
				|| strcasecmp(format, "ttc") == 0))
				{
					sprintf(path, "%s%s", dir, namelist[i]->d_name);
					len = strlen( path );
					filelist[*file_num] = (char *)malloc(sizeof(char)*(len+1)); 
					strcpy(filelist[*file_num], path);  
					++*file_num;
				}
			}
		}
		if(*file_num > 0)
			filelist = (char**)realloc(filelist, *file_num*sizeof(char*));
		else
		{
			free(filelist);
			filelist = NULL;
		}
	}
	for(i=0;i<n;++i)
		free(namelist[i]); 
	free(namelist); 
	
	return filelist;
}

void *change_fonttype()
{ 
	char info[256];
	for(;;) {
		sprintf(info, "(%d/%d) %s", current+1, total_files, filename[current]);
		Window_SetTitleText(window, info);
		Label_Refresh(text);
		sleep(1);
		++current;
		if(current >= total_files) {
			current = 0;
		}
	}
	LCUIThread_Exit(NULL);
}

int main(int argc, char *argv[])
{
	//thread_t t; 
	setenv( "LCUI_FONTFILE", "../fonts/consola/consola.ttf", TRUE );
	LCUI_Init();
	window = Widget_New("window");
	text = Widget_New("label"); 
	//filename = scan_fontfile("../fonts/", &total_files); 
	Widget_Resize(window, Size(320, 240));
	Window_SetTitleText( window, "测试字体" );
	Label_Text( text, 
		"<size=10px>10px: 汉字，abcdefghijklmn,opqrstuvwxyz</size>\n"
		"<size=11px>11px: 汉字，abcdefghijklmn,opqrstuvwxyz</size>\n" 
		"<size=12px>12px: 汉字，ABCDEFGHIJKLMN,OPQRSTUVWXYZ</size>\n"
		"<size=14px>14px: 汉字，ABCDEFGHIJKLMN,OPQRSTUVWXYZ</size>\n"
		"<size=16px>16px: 汉字，1234567890.:,;(*!?)</size>\n"
		"<size=18px>18px: 中国创造，慧及全球！</size>");
	Widget_SetAlign(text, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Window_ClientArea_Add(window, text);
	Widget_Show(text);
	Widget_Show(window); 
	//LCUIThread_Create(&t, NULL, change_fonttype, NULL);
	return LCUI_Main();
}
