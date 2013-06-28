/** ******************************************************************************
 * @file	listview.c
 * @brief	ListView Widget, used to displaying data With in list form.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	listview.c
 * @brief	列表视图部件，用于以列表形式展现数据.
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于						\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)				\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。	\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 * 用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/
 
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H

/** 每行数据中每列的数据 */
typedef struct TableColumnData_ {
	int column_id;		/**< 当前列对应的标识号 */
	const wchar_t *text;	/**< 当前列的文本内容 */
} TableColumnData;

typedef struct TableColumnInfo_ {
	int id;			/**< 列的标识号 */
	int width;		/**< 列的宽度 */
	const wchar_t *name;	/**< 列的名称 */
} TableColumnInfo;

typedef struct TableRowData_ {
	LCUI_Queue column_data;	/**< 当前行的每列数据 */
} TableRowData;

typedef struct Table_ {
	LCUI_Queue row_data;	/**< 行数据 */
	LCUI_Queue column_info;	/**< 列信息 */
} TableData;

int ListView_SetColumn(	LCUI_Widget *widget,
			TableRowData *target_row,
			const char *column_name,
			const char *item_name )
{
	return 0;
}

int ListView_AddColumn( LCUI_Widget *widget, const char *column_name )
{
	return 0;
}

TableRowData *ListView_NewRow( LCUI_Widget *widget )
{
	return 0;
}
