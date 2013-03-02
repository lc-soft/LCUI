/* ***************************************************************************
 * LCUI_Error.c -- LCUI‘s errors handling, at present, only macro definition
 * 
 * Copyright (C) 2013 by
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
 * LCUI_Error.h -- LCUI 的错误处理，目前只有宏定义
 *
 * 版权所有 (C) 2013 归属于
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


#define APP_ERROR_UNRECORDED_APP	"error: app was never recorded\n"
#define APP_ERROR_REGISTER_ERROR	"error: app register error\n"
#define ERROR_MALLOC_ERROR		"error: can not allocate memory space\n"

#define ERROR_CONVERT_ERROR		"error: convert error\n"

#define FT_INIT_ERROR                 "init error!\n"
#define FT_UNKNOWN_FILE_FORMAT        "unknown file format!\n"
#define FT_OPEN_FILE_ERROR            "open file error!\n"

#define INIT_ERROR_OPEN_FB_DEV_ERROR  "初始化错误:打开图形输出设备出错"
#define INIT_ERROR_FONTFILE_ERROR     "初始化错误:默认字体文件打开出错"

#define WIDGET_ERROR_GRAPH_ERROR	"部件错误:未为部件开辟内存空间!\n"
#define WIDGET_ERROR_TYPE_NOT_FOUND	"部件错误:未找到该类型的部件!\n"

#define LABLE_MALLOC_ERROR            "Lable_Text():无法分配足够的内存!\n"
#define WINDOWS_NUM_TOO_MUCH          "无法创建窗口!\n"
#define MALLOC_ERROR                  "无法分配足够的内存!\n" 
#define MSG_UNKNOWN_FORMAT            "未知文件格式!\n"
#define MSG_SHORT_FILE                "文件长度过短!\n"

#define WIDGET_TYPE_ERROR             "部件类型与支持的部件类型不一致!\n"
#define WIDGET_REALLOC_ERROR          "无法分配足够的内存用于存储窗口部件的数据！\n"
#define WIDGET_MALLOC_ERROR           "无法为该部件分配足够的内存空间！\n"

#define ADD_NEW_WIDGET_TYPE_ERROR	"添加新的部件类型出错！\n"

#define ERROR_WINDOW_CLOSED            -10

