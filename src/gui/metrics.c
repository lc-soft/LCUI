/* ***************************************************************************
 * metrics.c -- display related metrics operation set.
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
 * metrics.c -- 与显示器相关的度量操作集
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/metrics.h>

static struct LCUI_MetricsModule {
	float dpi;
	float density;
	float scaled_density;
	float scale;
} metrics;

float LCUIMetrics_ApplyDimension( LCUI_Style s )
{
	float value = 0.0;
	switch( s->type ) {
	case SVT_PX: value = s->val_px; break;
	case SVT_DIP: value = s->val_dip * metrics.density; break;
	case SVT_SP: value = s->val_sp * metrics.scaled_density; break;
	case SVT_PT: value = s->val_pt * metrics.dpi * (1.0f / 72); break;
	default: break;
	}
	return value * metrics.scale;
}

static float ComputeDensityByLevel( LCUI_DensityLevel level )
{
	float density = metrics.dpi / 96.0f;
	switch( level ) {
	case DENSITY_LEVEL_SMALL: density *= 0.75f; break;
	case DENSITY_LEVEL_LARGE: density *= 1.25f; break;
	case DENSITY_LEVEL_BIG: density *= 1.5f; break;
	case DENSITY_LEVEL_NORMAL:
	default: break;
	}
	return density;
}

void LCUIMetrics_SetDensity( float density )
{
	metrics.density = density;
}

void LCUIMetrics_SetScaledDensity( float density )
{
	metrics.scaled_density = density;
}

void LCUIMetrics_SetDensityLevel( LCUI_DensityLevel level )
{
	metrics.density = ComputeDensityByLevel( level );
}

void LCUIMetrics_SetScaledDensityLevel( LCUI_DensityLevel level )
{
	metrics.scaled_density = ComputeDensityByLevel( level );
}

void LCUIMetrics_SetDpi( float dpi )
{
	metrics.dpi = dpi;
	LCUIMetrics_SetDensityLevel( DENSITY_LEVEL_NORMAL );
	LCUIMetrics_SetScaledDensityLevel( DENSITY_LEVEL_NORMAL );
}

void LCUIMetrics_SetScale( float scale )
{
	metrics.scale = scale;
}

void LCUI_InitMetrics( void )
{
	metrics.scale = 1.0f;
	LCUIMetrics_SetDpi( 96.0f );
}

void LCUI_ExitMetrics( void )
{

}
