/* ***************************************************************************
 * metrics.h -- display related metrics operation set.
 *
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
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
 * metrics.h -- 与显示器相关的度量操作集
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_METRICS_H
#define LCUI_METRICS_H

LCUI_BEGIN_HEADER

typedef enum LCUI_DensityLevel {
	DENSITY_LEVEL_SMALL,
	DENSITY_LEVEL_NORMAL,
	DENSITY_LEVEL_LARGE,
	DENSITY_LEVEL_BIG
} LCUI_DensityLevel;

/** 转换成单位为 px 的度量值 */
LCUI_API float LCUIMetrics_Compute( float value, LCUI_StyleType type );

/** 将矩形中的度量值的单位转换为 px */
LCUI_API void LCUIMetrics_ComputeRectActual( LCUI_Rect *dst, const LCUI_RectF *src );

/** 转换成单位为 px 的实际度量值 */
LCUI_API int LCUIMetrics_ComputeActual( float value, LCUI_StyleType type );

/** 获取当前的全局缩放比例 */
LCUI_API float LCUIMetrics_GetScale( void );

/** 设置密度 */
LCUI_API void LCUIMetrics_SetDensity( float density );

/** 设置缩放密度 */
LCUI_API void LCUIMetrics_SetScaledDensity( float density );

/** 设置密度等级 */
LCUI_API void LCUIMetrics_SetDensityLevel( LCUI_DensityLevel level );

/** 设置缩放密度等级 */
LCUI_API void LCUIMetrics_SetScaledDensityLevel( LCUI_DensityLevel level );

/** 设置 DPI */
LCUI_API void LCUIMetrics_SetDpi( float dpi );

/** 设置全局缩放比例 */
LCUI_API void LCUIMetrics_SetScale( float scale );

LCUI_API void LCUI_InitMetrics( void );

void LCUI_ExitMetrics( void );

LCUI_END_HEADER

#endif
