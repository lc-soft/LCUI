/*
 * widget_background.c -- The widget background style processing module.
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/image.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>

#define ComputeActual LCUIMetrics_ComputeActual

typedef struct ImageCacheRec_ {
	char *path;
	LCUI_Graph image;
	LinkedList refs;
} ImageCacheRec, *ImageCache;

typedef struct ImageRefRec_ {
	LCUI_Widget widget;
	ImageCache cache;
} ImageRefRec, *ImageRef;

struct LCUI_WidgetBackgroundModule {
	LCUI_BOOL active;
	DictType dtype;
	Dict *images;
	RBTree refs;
} self;

static void DestroyImageCache(ImageCache cache)
{
	LinkedListNode *node;
	while ((node = LinkedList_GetNode(&cache->refs, 0))) {
		LCUI_Widget w = node->data;
		RBTree_CustomErase(&self.refs, node->data);
		Widget_UnsetStyle(w, key_background_image);
		Graph_Init(&w->computed_style.background.image);
		LinkedList_DeleteNode(&cache->refs, node);
	}
	Graph_Free(&cache->image);
	free(cache->path);
	cache->path = NULL;
	free(cache);
}

static void ImageCacheDestructor(void *privdata, void *data)
{
	DestroyImageCache(data);
}

static void AddImageRef(LCUI_Widget widget, ImageCache cache)
{
	ASSIGN(ref, ImageRef);
	ref->cache = cache;
	ref->widget = widget;
	RBTree_CustomInsert(&self.refs, widget, ref);
	LinkedList_Append(&cache->refs, widget);
}

static ImageRef GetImageRef(LCUI_Widget widget)
{
	return RBTree_CustomGetData(&self.refs, widget);
}

static void DeleteImageRef(LCUI_Widget widget)
{
	ImageRef ref;
	ImageCache cache;
	LinkedListNode *node;
	ref = GetImageRef(widget);
	if (!ref) {
		return;
	}
	cache = ref->cache;
	for (LinkedList_Each(node, &cache->refs)) {
		LCUI_Widget w = node->data;
		if (w != widget) {
			continue;
		}
		RBTree_CustomErase(&self.refs, node->data);
		Widget_UnsetStyle(w, key_background_image);
		Graph_Init(&w->computed_style.background.image);
		LinkedList_DeleteNode(&cache->refs, node);
		break;
	}
	RBTree_CustomErase(&self.refs, widget);
	if (cache->refs.length < 1) {
		Dict_Delete(self.images, cache->path);
	}
}

static void ExecLoadImage(void *arg1, void *arg2)
{
	char *path = arg2;
	LCUI_Graph image;
	LCUI_Widget w = arg1;
	ImageCache cache;

	Graph_Init(&image);
	if (LCUI_ReadImageFile(path, &image) != 0) {
		return;
	}
	cache = NEW(ImageCacheRec, 1);
	cache->image = image;
	cache->path = strdup2(path);
	LinkedList_Init(&cache->refs);
	if (Dict_Add(self.images, cache->path, cache) == 0) {
		AddImageRef(w, cache);
	} else {
		DestroyImageCache(cache);
	}
	Graph_Quote(&w->computed_style.background.image, &cache->image, NULL);
	Widget_InvalidateArea(w, NULL, SV_BORDER_BOX);
}

static int OnCompareWidget(void *data, const void *keydata)
{
	ImageRef ref = data;
	if (ref->widget == keydata) {
		return 0;
	}
	if ((void *)ref->widget > keydata) {
		return 1;
	}
	return -1;
}

static void AsyncLoadImage(LCUI_Widget widget, const char *path)
{
	ImageRef ref;
	ImageCache cache;
	LCUI_TaskRec task = { 0 };
	LCUI_Style s = &widget->style->sheet[key_background_image];

	if (!self.active) {
		return;
	}
	if (Widget_CheckStyleType(widget, key_background_image, string)) {
		ref = GetImageRef(widget);
		if (ref && strcmp(ref->cache->path, s->string) == 0) {
			return;
		}
		if (ref) {
			DeleteImageRef(widget);
		}
	}
	cache = Dict_FetchValue(self.images, path);
	if (cache) {
		AddImageRef(widget, cache);
		Graph_Quote(&widget->computed_style.background.image,
			    &cache->image, NULL);
		Widget_InvalidateArea(widget, NULL, SV_BORDER_BOX);
		return;
	}
	task.func = ExecLoadImage;
	task.arg[0] = widget;
	task.arg[1] = strdup2(path);
	task.destroy_arg[1] = free;
	LCUI_PostAsyncTask(&task);
}

void LCUIWidget_InitImageLoader(void)
{
	RBTree_Init(&self.refs);
	self.dtype = DictType_StringKey;
	self.dtype.valDestructor = ImageCacheDestructor;
	self.images = Dict_Create(&self.dtype, NULL);
	RBTree_OnCompare(&self.refs, OnCompareWidget);
	RBTree_OnDestroy(&self.refs, free);
	self.active = TRUE;
}

void LCUIWidget_FreeImageLoader(void)
{
	Dict_Release(self.images);
	RBTree_Destroy(&self.refs);
	self.images = NULL;
	self.active = FALSE;
}

void Widget_InitBackground(LCUI_Widget w)
{
	LCUI_BackgroundStyle *bg;
	bg = &w->computed_style.background;
	bg->color = RGB(255, 255, 255);
	Graph_Init(&bg->image);
	bg->size.using_value = TRUE;
	bg->size.value = SV_AUTO;
	bg->position.using_value = TRUE;
	bg->position.value = SV_AUTO;
}

void Widget_DestroyBackground(LCUI_Widget w)
{
	Widget_UnsetStyle(w, key_background_image);
	Graph_Init(&w->computed_style.background.image);
	if (Widget_CheckStyleType(w, key_background_image, string)) {
		DeleteImageRef(w);
	}
}

void Widget_ComputeBackgroundStyle(LCUI_Widget widget)
{
	LCUI_Style s;
	LCUI_StyleSheet ss = widget->style;
	LCUI_BackgroundStyle *bg = &widget->computed_style.background;
	int key = key_background_start;

	for (; key <= key_background_end; ++key) {
		s = &ss->sheet[key];
		switch (key) {
		case key_background_color:
			if (s->is_valid) {
				bg->color = s->color;
			} else {
				bg->color.value = 0;
			}
			break;
		case key_background_image:
			if (!s->is_valid) {
				Graph_Init(&bg->image);
				break;
			}
			switch (s->type) {
			case LCUI_STYPE_STRING:
				AsyncLoadImage(widget, s->string);
				break;
			case LCUI_STYPE_IMAGE:
				if (!s->image) {
					Graph_Init(&bg->image);
					break;
				}
				Graph_Quote(&bg->image, s->image, NULL);
				DeleteImageRef(widget);
			default:
				break;
			}
			break;
		case key_background_position:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->position.using_value = TRUE;
				bg->position.value = s->val_style;
			} else {
				bg->position.using_value = FALSE;
				bg->position.value = 0;
			}
			break;
		case key_background_position_x:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->position.using_value = FALSE;
				bg->position.x = *s;
			}
			break;
		case key_background_position_y:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->position.using_value = FALSE;
				bg->position.y = *s;
			}
			break;
		case key_background_size:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->size.using_value = TRUE;
				bg->size.value = s->val_style;
			} else {
				bg->size.using_value = FALSE;
				bg->size.value = 0;
			}
			break;
		case key_background_size_width:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->size.using_value = FALSE;
				bg->size.width = *s;
			}
			break;
		case key_background_size_height:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->size.using_value = FALSE;
				bg->size.height = *s;
			}
			break;
		default:
			break;
		}
	}
}

void Widget_ComputeBackground(LCUI_Widget w, LCUI_Background *out)
{
	LCUI_StyleType type;
	LCUI_RectF *box = &w->box.border;
	LCUI_BackgroundStyle *bg = &w->computed_style.background;
	float scale, x = 0, y = 0, width, height;

	/* 计算背景图应有的尺寸 */
	if (bg->size.using_value) {
		switch (bg->size.value) {
		case SV_CONTAIN:
			width = box->width;
			scale = 1.0f * bg->image.width / width;
			height = bg->image.height / scale;
			if (height > box->height) {
				height = box->height;
				scale = 1.0f * bg->image.height / box->height;
				width = bg->image.width / scale;
			}
			break;
		case SV_COVER:
			width = box->width;
			scale = 1.0f * bg->image.width / width;
			height = bg->image.height / scale;
			if (height < box->height) {
				height = box->height;
				scale = 1.0f * bg->image.height / height;
				width = bg->image.width / scale;
			}
			x = (box->width - width) / 2.0f;
			y = (box->height - height) / 2.0f;
			break;
		case SV_AUTO:
		default:
			width = (float)bg->image.width;
			height = (float)bg->image.height;
			break;
		}
		out->position.x = ComputeActual(x, LCUI_STYPE_PX);
		out->position.y = ComputeActual(y, LCUI_STYPE_PX);
		out->size.width = ComputeActual(width, LCUI_STYPE_PX);
		out->size.height = ComputeActual(height, LCUI_STYPE_PX);
	} else {
		type = LCUI_STYPE_PX;
		switch (bg->size.width.type) {
		case LCUI_STYPE_SCALE:
			width = box->width * bg->size.width.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			width = (float)bg->image.width;
			break;
		default:
			width = bg->size.width.value;
			type = bg->size.width.type;
			break;
		}
		out->size.width = ComputeActual(width, type);
		type = LCUI_STYPE_PX;
		switch (bg->size.height.type) {
		case LCUI_STYPE_SCALE:
			height = box->height * bg->size.height.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			height = (float)bg->image.height;
			break;
		default:
			height = (float)bg->size.height.value;
			break;
		}
		out->size.height = ComputeActual(height, type);
	}
	/* 计算背景图的像素坐标 */
	if (bg->position.using_value) {
		switch (bg->position.value) {
		case SV_TOP:
		case SV_TOP_CENTER:
			x = (box->width - width) / 2;
			y = 0;
			break;
		case SV_TOP_RIGHT:
			x = box->width - width;
			y = 0;
			break;
		case SV_CENTER_LEFT:
			x = 0;
			y = (box->height - height) / 2;
			break;
		case SV_CENTER:
		case SV_CENTER_CENTER:
			x = (box->width - width) / 2;
			y = (box->height - height) / 2;
			break;
		case SV_CENTER_RIGHT:
			x = box->width - width;
			y = (box->height - height) / 2;
			break;
		case SV_BOTTOM_LEFT:
			x = 0;
			y = box->height - height;
			break;
		case SV_BOTTOM_CENTER:
			x = (box->width - width) / 2;
			y = box->height - height;
			break;
		case SV_BOTTOM_RIGHT:
			x = box->width - width;
			y = box->height - height;
			break;
		case SV_TOP_LEFT:
		default:
			break;
		}
		out->position.x = ComputeActual(x, LCUI_STYPE_PX);
		out->position.y = ComputeActual(y, LCUI_STYPE_PX);
	} else {
		type = LCUI_STYPE_PX;
		switch (bg->position.x.type) {
		case LCUI_STYPE_SCALE:
			x = box->width - width;
			x = x * bg->position.x.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			break;
		default:
			x = bg->position.x.value;
			type = bg->position.x.type;
			break;
		}
		out->position.x = ComputeActual(x, type);
		type = LCUI_STYPE_PX;
		switch (bg->position.y.type) {
		case LCUI_STYPE_SCALE:
			y = box->height - height;
			y = y * bg->position.y.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			break;
		default:
			y = bg->position.y.value;
			type = bg->position.y.type;
			break;
		}
		out->position.y = ComputeActual(y, type);
	}
	out->color = bg->color;
	out->image = &bg->image;
	out->repeat.x = bg->repeat.x;
	out->repeat.y = bg->repeat.y;
}

void Widget_PaintBakcground(LCUI_Widget w, LCUI_PaintContext paint,
			    LCUI_WidgetActualStyle style)
{
	LCUI_Rect box;
	box.x = style->padding_box.x - style->canvas_box.x;
	box.y = style->padding_box.y - style->canvas_box.y;
	box.width = style->padding_box.width;
	box.height = style->padding_box.height;
	Background_Paint(&style->background, &box, paint);
}
