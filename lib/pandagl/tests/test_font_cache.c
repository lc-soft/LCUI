/*
 * lib/pandagl/tests/test_font_cache.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <pandagl.h>
#include <ctest.h>

/*
 * 字形位图缓存的功能测试：
 * - 同一 (face, ch, size) 第二次请求应命中缓存，返回相同指针；
 * - 不同 size 应得到不同 entry；
 * - 渲染失败的字符回退到 .notdef，不应崩溃。
 */

static void should_return_same_pointer_on_cache_hit(void)
{
        const pd_glyph_bitmap_t *bmp1 = NULL;
        const pd_glyph_bitmap_t *bmp2 = NULL;

        ctest_equal_int("first lookup should succeed",
                        pd_font_cache_get_bitmap('A', -1, 14, &bmp1), 0);
        ctest_equal_int("second lookup should succeed",
                        pd_font_cache_get_bitmap('A', -1, 14, &bmp2), 0);
        ctest_equal_bool("cached bitmaps should share the same address",
                         bmp1 == bmp2, true);
}

static void should_treat_different_sizes_as_different_entries(void)
{
        const pd_glyph_bitmap_t *bmp1 = NULL;
        const pd_glyph_bitmap_t *bmp2 = NULL;

        pd_font_cache_get_bitmap('B', -1, 12, &bmp1);
        pd_font_cache_get_bitmap('B', -1, 24, &bmp2);
        ctest_equal_bool("different sizes should yield different entries",
                         bmp1 != NULL && bmp2 != NULL && bmp1 != bmp2, true);
}

static void should_not_crash_on_unknown_codepoint(void)
{
        const pd_glyph_bitmap_t *bmp = NULL;
        /* 超出 BMP 的字符大概率没有 glyph，应回退而非崩溃 */
        int ret = pd_font_cache_get_bitmap(0x10FFFD, -1, 14, &bmp);
        ctest_equal_bool("should return non-positive code", ret <= 0, true);
}

static void should_evict_least_recently_used(void)
{
        const pd_glyph_bitmap_t *bmp = NULL;
        size_t before;

        /* 先用很大上限，预热若干 entry */
        pd_font_cache_set_capacity(1024);
        pd_font_cache_get_bitmap('X', -1, 10, &bmp);
        pd_font_cache_get_bitmap('Y', -1, 10, &bmp);
        pd_font_cache_get_bitmap('Z', -1, 10, &bmp);
        before = pd_font_cache_count();
        ctest_equal_bool("should hold at least 3 entries", before >= 3, true);

        /* 把上限缩到 2，应立即驱逐 */
        pd_font_cache_set_capacity(2);
        ctest_equal_uint("count should drop to capacity",
                         (unsigned)pd_font_cache_count(), 2u);

        /* 添加新 entry 时仍不超过上限 */
        pd_font_cache_get_bitmap('W', -1, 10, &bmp);
        ctest_equal_uint("count should stay at capacity after new insert",
                         (unsigned)pd_font_cache_count(), 2u);

        /* 恢复无限制 */
        pd_font_cache_set_capacity(0);
}

void test_pandagl_font_cache(void)
{
        pd_font_library_init();
        ctest_describe("glyph cache hit returns same pointer",
                       should_return_same_pointer_on_cache_hit);
        ctest_describe("glyph cache differentiates by size",
                       should_treat_different_sizes_as_different_entries);
        ctest_describe("glyph cache handles unknown codepoint",
                       should_not_crash_on_unknown_codepoint);
        ctest_describe("glyph cache lru eviction",
                       should_evict_least_recently_used);
        pd_font_library_destroy();
}
