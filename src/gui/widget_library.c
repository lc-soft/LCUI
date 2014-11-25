/* ***************************************************************************
 * widget_library.c -- LCUI widget library management module.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_library.c -- LCUI部件库管理模块
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/widget_build.h>

static LCUI_RBTree widget_class_library;

static int CompareName( void *data, const void *keydata )
{
	return strcmp(*(char**)data, (const char*)keydata);
}

static void OnDestroyWidgetClass( void *arg )
{
	LCUI_WidgetClass *class_data = (LCUI_WidgetClass*)arg;
	free( class_data->name );
	RBTree_Destroy( &class_data->func_records );
}

void LCUIWidget_InitLibrary(void)
{
	RBTree_Init( &widget_class_library );
	RBTree_OnJudge( &widget_class_library, CompareName );
	RBTree_SetDataNeedFree( &widget_class_library, FALSE );
	RBTree_OnDestroy( &widget_class_library, OnDestroyWidgetClass );
}

void LCUIWidget_DestroyLibrary(void)
{
	RBTree_Destroy( &widget_class_library );
}

/** 获取部件类数据 */
LCUI_WidgetClass LCUIWidget_GetClass( const char *class_name )
{
	return (LCUI_WidgetClass*)RBTree_CustomGetData(
		&widget_class_library, class_name
	);
}

/** 添加一个部件类型 */
LCUI_WidgetClass LCUIWidget_AddClass( const char *class_name )
{
	int len;
	LCUI_WidgetClass *class_data;

	class_data = LCUIWidget_GetClass( class_name );
	if( class_data ) {
		return class_data;
	}

	len = strlen( class_name );
	class_data = (LCUI_WidgetClass*)malloc(sizeof(LCUI_WidgetClass));
	class_data->methods.init = NULL;
	class_data->methods.destroy = NULL;
	class_data->methods.paint = NULL;
	class_data->task_handler = NULL;
	class_data->name = (char*)malloc(sizeof(char)*(len+1));
	strcpy( class_data->name, class_name );
	RBTree_Init( &class_data->func_records );
	RBTree_SetDataNeedFree( &class_data->func_records, FALSE );
	RBTree_CustomInsert( &widget_class_library, class_name, class_data );
	return class_data;
}

/** 移除一个部件类型 */
int LCUIWidget_RemoveClass( const char *class_name )
{
	return RBTree_CustomErase( &widget_class_library, class_name );
}
