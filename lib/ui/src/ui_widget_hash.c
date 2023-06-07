#include <ui/base.h>
#include <ui/hash.h>

void ui_widget_generate_self_hash(ui_widget_t* widget)
{
	int i;
	unsigned hash = 1080;
	ui_widget_t* w;

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
		if (w->extra && w->extra->rules.cache_children_style) {
			break;
		}
	}
	widget->hash = hash;
}

void ui_widget_generate_hash(ui_widget_t* w)
{
	list_node_t *node;

	ui_widget_generate_self_hash(w);
	for (list_each(node, &w->children)) {
		ui_widget_generate_hash(node->data);
	}
}

size_t ui_widget_export_hash(ui_widget_t* w, unsigned *hash_list, size_t len)
{
	size_t count = 0;
	ui_widget_t* child;

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

size_t ui_widget_import_hash(ui_widget_t* w, unsigned *hash_list, size_t maxlen)
{
	size_t count = 0;
	ui_widget_t* child;

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
