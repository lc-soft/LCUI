/*
 * widget_hash.c -- Generates a hash for the component to cache its stylesheet
 *
 * Copyright (c) 2020, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget_hash.h>

void Widget_GenerateSelfHash(LCUI_Widget widget)
{
	int i;
	unsigned hash = 1080;
	LCUI_Widget w;

	for (w = widget; w; w = w->parent) {
		if (w != widget) {
			hash = strhash(hash, " ");
		}
		if (w->type) {
			hash = strhash(hash, w->type);
		} else {
			hash = strhash(hash, "*");
		}
		if (w->id) {
			hash = strhash(hash, "#");
			hash = strhash(hash, w->id);
		}
		if (w->classes) {
			for (i = 0; w->classes[i]; ++i) {
				hash = strhash(hash, ".");
				hash = strhash(hash, w->classes[i]);
			}
		}
		if (w->status) {
			for (i = 0; w->status[i]; ++i) {
				hash = strhash(hash, ":");
				hash = strhash(hash, w->status[i]);
			}
		}
		if (w->rules && w->rules->cache_children_style) {
			break;
		}
	}
	widget->hash = hash;
}

void Widget_GenerateHash(LCUI_Widget w)
{
	LinkedListNode *node;

	Widget_GenerateSelfHash(w);
	for (LinkedList_Each(node, &w->children)) {
		Widget_GenerateHash(node->data);
	}
}

size_t Widget_SetHashList(LCUI_Widget w, unsigned *hash_list, size_t len)
{
	size_t count = 0;
	LCUI_Widget child;

	child = w;
	if (hash_list) {
		child->hash = hash_list[count];
	}
	++count;
	if (len > 0 && count >= len) {
		return count;
	}
	while (child->children.length > 0) {
		child = child->children.head.next->data;
	}
	while (child != w) {
		while (child->children.length > 0) {
			child = child->children.head.next->data;
		}
		if (hash_list) {
			child->hash = hash_list[count];
		}
		++count;
		if (len > 0 && count >= len) {
			break;
		}
		if (child->node.next) {
			child = child->node.next->data;
			continue;
		}
		do {
			child = child->parent;
			if (child == w) {
				break;
			}
			if (child->node.next) {
				child = child->node.next->data;
				break;
			}
		} while (1);
	}
	return count;
}

size_t Widget_GetHashList(LCUI_Widget w, unsigned *hash_list, size_t maxlen)
{
	size_t count = 0;
	LCUI_Widget child;

	child = w;
	if (hash_list) {
		hash_list[count] = child->hash;
	}
	++count;
	if (maxlen > 0 && count >= maxlen) {
		return count;
	}
	while (child->children.length > 0) {
		child = child->children.head.next->data;
	}
	while (child != w) {
		while (child->children.length > 0) {
			child = child->children.head.next->data;
		}
		if (hash_list) {
			hash_list[count] = child->hash;
		}
		++count;
		if (maxlen > 0 && count >= maxlen) {
			break;
		}
		if (child->node.next) {
			child = child->node.next->data;
			continue;
		}
		do {
			child = child->parent;
			if (child == w) {
				break;
			}
			if (child->node.next) {
				child = child->node.next->data;
				break;
			}
		} while (1);
	}
	return count;
}
