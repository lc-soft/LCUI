/* ***************************************************************************
 * LCUI_Misc.h -- no specific categories of miscellaneous function
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_Misc.h -- 没有具体分类的杂项功能
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#ifndef __LCUI_MISC_H__
#define __LCUI_MISC_H__

LCUI_BEGIN_HEADER

/* 秒级延时 */
void LCUI_Sleep( unsigned int s );

/* 毫秒级延时 */
void LCUI_MSleep( unsigned int ms );

/******************************** Misc *********************************/
int Check_Option(int result, int option);
/* 功能：检测result是否包含option */

void Padding_Init( LCUI_Padding *padding );

void Margin_Init( LCUI_Margin *margin );

LCUI_Pos Pos(int x, int y);
/* 功能：转换成LCUI_Pos类型 */ 

LCUI_Size Size(int w, int h);
/* 功能：转换成LCUI_Size类型 */ 

int Size_Cmp(LCUI_Size a, LCUI_Size b);
/* 
 * 功能：对比两个尺寸
 * 说明：a大于b，返回1， b大于a，返回-1，相等则返回0
 * */ 

LCUI_Pos Align_Get_Pos(LCUI_Size container, LCUI_Size child, int align);
/* 功能：根据容器尺寸，区域尺寸以及对齐方式，获取该区域的位置 */ 

LCUI_Padding Padding(int top, int bottom, int left, int right);

LCUI_Pos Pos_Add(LCUI_Pos a, LCUI_Pos b);
/* 功能：求两个LCUI_Pos类型变量的和 */ 

int Pos_Cmp(LCUI_Pos a, LCUI_Pos b);
/* 功能：对比两个坐标是否一致 */ 

LCUI_Pos Pos_Sub(LCUI_Pos a, LCUI_Pos b);
/* 功能：求两个LCUI_Pos类型变量的差 */ 
/*************************** End Misc *********************************/

/****************************** Rect **********************************/
void Rect_Init(LCUI_Rect *rect);
/* 功能：初始化矩形区域的数据 */ 

int Rect_Cross_Overlay(LCUI_Rect a, LCUI_Rect b);
/* 
 * 功能：检测两个矩形是否成十字架式叠加 
 * 返回值：
 *  1 a竖，b横
 * -1 a衡，b竖
 *  0 不叠加
 **/

int Get_Cut_Area(LCUI_Size container, LCUI_Rect rect, LCUI_Rect *cut);
/* 
 * 功能：获取需裁剪的区域
 * 说明：指定容器尺寸和容器中的区域位置及尺寸，即可得到该区域中需要进行裁剪区域
 *  */

LCUI_Rect Get_Valid_Area(LCUI_Size container, LCUI_Rect rect);
/* 
 * 功能：获取指定区域在容器中的有效显示区域 
 * 说明：指定容器的区域大小，再指定容器中的区域位置及大小，就能得到该容器实际能显示
 * 出的该区域范围。
 * */ 

int Rect_Include_Rect (LCUI_Rect a, LCUI_Rect b);
/*
 * 功能：检测两个矩形中，A矩形是否包含B矩形
 * 返回值：两不矩形属于包含关系返回1，否则返回0。
 * */ 
 
int Cut_Overlay_Rect (	LCUI_Rect old_rect, LCUI_Rect new_rect, 
					LCUI_Queue *rq	);
/*
 * 功能：将有重叠部分的两个矩形，进行分割，并得到分割后的矩形
 * 说明：主要用于局部区域刷新里，添加的需刷新的区域有可能会与已添加的区域重叠，为避免
 * 重复刷新同一块区域，需要在添加时对矩形进行分割，得到完全重叠和不重叠的矩形。
 * 参数说明：
 * old ： 已存在的矩形区域
 * new ： 将要添加的矩形区域
 * rq  ： 指向矩形的队列的指针
 * 注意！传递参数时，请勿颠倒old和new位置。
 **/ 

int Get_Overlay_Rect(LCUI_Rect a, LCUI_Rect b, LCUI_Rect *out);
/* 功能：获取两矩形重叠部分的矩形 */ 

LCUI_Rect Rect (int x, int y, int width, int height);
/* 功能：将数值转换成LCUI_Rect型结构体 */ 

int Rect_Inside_Point (LCUI_Pos pos, LCUI_Rect rect);
/* 功能：检测一个点是否被矩形包含 */ 

int Check_Rect_Overlap (	int ax, int ay, int aw, int ah, 
				int bx, int by, int bw, int bh );
/*
 * 功能：检测两个矩形是否重叠
 * 参数说明：
 * ax ：矩形A的左上角起点所在x轴的坐标
 * ay ：矩形A的左上角起点所在y轴的坐标
 * aw ：矩形A的宽度
 * ah ：矩形A的高度
 * bx ：矩形B的左上角起点所在x轴的坐标
 * by ：矩形B的左上角起点所在y轴的坐标
 * bw ：矩形B的宽度
 * bh ：矩形B的高度
 * 返回值：两不重叠返回0，重叠则返回1。
 */ 

int Rect_Is_Overlay (LCUI_Rect a, LCUI_Rect b);
/* 功能：检测两个矩形是否重叠 */ 

int Rect_Equal (LCUI_Rect a, LCUI_Rect b);
/* 功能：判断两个矩形是否相等 */ 

int Rect_Valid(LCUI_Rect r);
/* 功能：判断矩形是否有效 */ 
/************************** End Rect **********************************/


/***************************** String *********************************/
#ifdef LCUI_BUILD_IN_WIN32
#define strcasecmp(str1, str2)	lcui_strcasecmp(str1, str2)
#endif

/* 不区分大小写，对比两个字符串 */
int lcui_strcasecmp( const char *str1, const char *str2 );

void String_Init(LCUI_String *in);
/* 功能：初始化String结构体中的数据 */ 

void Strcpy (LCUI_String * des, const char *src);
/* 功能：拷贝字符串至String结构体数据中 */ 

int Strcmp(LCUI_String *str1, const char *str2);
/* 功能：对比str1与str2 */ 

int LCUI_Strcmp(LCUI_String *str1, LCUI_String *str2);
/* LCUI_String 字符串对比 */ 

int LCUI_Strcpy(LCUI_String *str1, LCUI_String *str2);
/* LCUI_String 字符串拷贝 */ 

void String_Free(LCUI_String *in);

void WChar_T_Free(LCUI_WChar_T *ch);

void WString_Free(LCUI_WString *str);
/************************** End String ********************************/


void PX_P_t_init( PX_P_t *combo_num );
/* 初始化PX_P_t */ 

void PX_PT_t_init( PX_PT_t *combo_num );
/* 初始化PX_PT_t */ 

int get_PX_P_t( char *str, PX_P_t *combo_num );
/* 根据传入的字符串，获取字符串实际表达的数值，确定数值的单位是PX还是百分比 */ 

int get_PX_PT_t( char *str, PX_PT_t *combo_num );
/* 根据传入的字符串，获取字符串实际表达的数值，确定数值的单位是PX还是PT */ 

void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out);
/* 
 * 功能：通过内存中的图像数据，捕获将在屏幕上显示的图像
 * 说明：效率较低，因为需要进行合成。
 *  */ 

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out);
/* 
 * 功能：直接读取帧缓冲中的图像数据
 * 说明：效率较高，但捕获的图像有可能会有问题。
 * */

int start_record_screen( LCUI_Rect area );
/* 录制屏幕指定区域的内容 */

int end_catch_screen();
/* 结束录制 */

LCUI_END_HEADER

#endif
