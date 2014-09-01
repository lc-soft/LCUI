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

typedef struct TypeData {
	char *name;
	LCUI_RBTree func_records;
} TypeData;

static LCUI_RBTree widget_type_records;

static int CompareName( void *data, const void *keydata )
{
	return strcmp(*(char**)data, (const char*)keydata);
}

static void OnDestroyWidgetType( void *arg )
{
	TypeData *type_data = (TypeData*)arg;
	free( type_data->name );
	RBTree_Destroy( &type_data->func_records );
}

void LCUIWidget_InitTypeLibrary(void)
{
	RBTree_Init( &widget_type_records );
	RBTree_OnJudge( &widget_type_records, CompareName );
	RBTree_SetDataNeedFree( &widget_type_records, FALSE );
	RBTree_OnDestroy( &widget_type_records, OnDestroyWidgetType );
}

void LCUIWidget_ClearTypeLibrary(void)
{
	RBTree_Destroy( &widget_type_records );
}

/** 添加一个部件类型 */
int LCUIWidget_AddType( const char *widget_type )
{
	int len;
	TypeData *type_data;

	type_data = (TypeData*)RBTree_CustomGetData(
		&widget_type_records, widget_type
	);
	if( type_data ) {
		return -1;
	}

	len = strlen( widget_type );
	type_data = (TypeData*)malloc(sizeof(TypeData));
	type_data->name = (char*)malloc(sizeof(char)*(len+1));
	strcpy( type_data->name, widget_type );
	RBTree_Init( &type_data->func_records );
	RBTree_SetDataNeedFree( &type_data->func_records, FALSE );
	RBTree_CustomInsert( &widget_type_records, widget_type, type_data );
	return 0;
}

/** 移除一个部件类型 */
int LCUIWidget_RemoveType( const char *widget_type )
{
	return RBTree_CustomErase( &widget_type_records, widget_type );
}

/** 为指定类型的部件设置函数 */
int LCUIWidget_SetFunc( const char *widget_type, const char *func_type,
			void(*func)(LCUI_Widget*) )
{
	TypeData *type_data;
	LCUI_RBTreeNode *node;

	type_data = (TypeData*)RBTree_CustomGetData(
		&widget_type_records, widget_type
	);
	if( !type_data ) {
		return -1;
	}
	node = RBTree_CustomSearch( &type_data->func_records, func_type );
	if( node ) {
		node->data = func;
		return 0;
	}
	RBTree_CustomInsert( &type_data->func_records, func_type, func );
	return 0;
}

/** 调用指定类型部件的函数 */
int Widget_Call( LCUI_Widget *widget, const char *func_type )
{
	TypeData *type_data;
	void (*func)(LCUI_Widget*);

	type_data = (TypeData*)RBTree_CustomGetData(
		&widget_type_records, widget->type_name
	);
	if( !type_data ) {
		return -1;
	}
	func = (void (*)(LCUI_Widget*))RBTree_CustomGetData( 
		&type_data->func_records, func_type
	);
	if( func ) {
		func( widget );
		return 0;
	}
	return -2;
}
