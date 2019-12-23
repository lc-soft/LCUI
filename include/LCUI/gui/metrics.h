/*
 * metrics.h -- Display related metrics operation set.
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

#ifndef LCUI_METRICS_H
#define LCUI_METRICS_H

LCUI_BEGIN_HEADER

typedef struct LCUI_MetricsRec_ {
	float dpi;
	float density;
	float scaled_density;
	float scale;
} LCUI_MetricsRec, *LCUI_Metrics;

typedef enum LCUI_DensityLevel {
	DENSITY_LEVEL_SMALL,
	DENSITY_LEVEL_NORMAL,
	DENSITY_LEVEL_LARGE,
	DENSITY_LEVEL_BIG
} LCUI_DensityLevel;

/** 转换成单位为 px 的度量值 */
LCUI_API float LCUIMetrics_Compute(float value, LCUI_StyleType type);

/** 将矩形中的度量值的单位转换为 px */
LCUI_API void LCUIMetrics_ComputeRectActual(LCUI_Rect *dst, const LCUI_RectF *src);

/** 转换成单位为 px 的实际度量值 */
LCUI_API int LCUIMetrics_ComputeActual(float value, LCUI_StyleType type);

/** 获取当前的全局缩放比例 */
LCUI_API float LCUIMetrics_GetScale(void);

/** 设置密度 */
LCUI_API void LCUIMetrics_SetDensity(float density);

/** 设置缩放密度 */
LCUI_API void LCUIMetrics_SetScaledDensity(float density);

/** 设置密度等级 */
LCUI_API void LCUIMetrics_SetDensityLevel(LCUI_DensityLevel level);

/** 设置缩放密度等级 */
LCUI_API void LCUIMetrics_SetScaledDensityLevel(LCUI_DensityLevel level);

/** 设置 DPI */
LCUI_API void LCUIMetrics_SetDpi(float dpi);

/** 设置全局缩放比例 */
LCUI_API void LCUIMetrics_SetScale(float scale);

LCUI_API void LCUI_InitMetrics(void);

LCUI_API const LCUI_MetricsRec *LCUI_GetMetrics(void);

void LCUI_FreeMetrics(void);

LCUI_END_HEADER

#endif
