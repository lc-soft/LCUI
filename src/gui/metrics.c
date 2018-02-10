/*
 * metrics.c -- Display related metrics operation set.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/metrics.h>

static struct LCUI_MetricsModule {
	float dpi;
	float density;
	float scaled_density;
	float scale;
} metrics;

float LCUIMetrics_Compute( float value, LCUI_StyleType type )
{
	switch( type ) {
	case SVT_PX: break;
	case SVT_DIP: value = value * metrics.density; break;
	case SVT_SP: value = value * metrics.scaled_density; break;
	case SVT_PT: value = value * metrics.dpi / 72.0f; break;
	default: value = 0; break;
	}
	return value;
}

int LCUIMetrics_ComputeActual( float value, LCUI_StyleType type )
{
	return iround( LCUIMetrics_Compute( value, type ) * metrics.scale );
}

void LCUIMetrics_ComputeRectActual( LCUI_Rect *dst, const LCUI_RectF *src )
{
	dst->x = LCUIMetrics_ComputeActual( src->x, SVT_PX );
	dst->y = LCUIMetrics_ComputeActual( src->y, SVT_PX );
	dst->width = LCUIMetrics_ComputeActual( src->width, SVT_PX );
	dst->height = LCUIMetrics_ComputeActual( src->height, SVT_PX );
}

float LCUIMetrics_GetScale( void )
{
	return metrics.scale;
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
	scale = max( 0.5f, scale );
	scale = min( 5.0f, scale );
	metrics.scale = scale;
}

void LCUI_InitMetrics( void )
{
	metrics.scale = 1.0f;
	LCUIMetrics_SetDpi( 96.0f );
}

void LCUI_FreeMetrics( void )
{

}
