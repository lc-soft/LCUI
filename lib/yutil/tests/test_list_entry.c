#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/list_entry.h"

struct __test_list_entry_t {
	list_entry_t entry_node;
	size_t data;
};

typedef struct __test_list_entry_t test_list_entry_t;

void test_list_entry(void)
{
	test_list_entry_t entries[8] = { { { 0 }, 0 }, { { 0 }, 1 },
					 { { 0 }, 2 }, { { 0 }, 3 },
					 { { 0 }, 4 }, { { 0 }, 5 },
					 { { 0 }, 6 }, { { 0 }, 7 } };

	// head init
	list_entry_head_t list;
	list_entry_head_init(&list, test_list_entry_t, entry_node);
	it_b("list_entry_head_init() should work", !list.length, true);

	// entry init
	list_entry_t list_entry1;
	list_entry_init(&list_entry1);

	// add head
	list_entry_add_head(&list, &entries[3].entry_node);
	list_entry_add_head(&list, &entries[2].entry_node);
	list_entry_add_head(&list, &entries[1].entry_node);
	it_i("list_entry_add_head() should work", (int)list_entry_size(&list),
	     3);

	// add tail
	list_entry_add_tail(&list, &entries[4].entry_node);
	list_entry_add_tail(&list, &entries[5].entry_node);
	list_entry_add_tail(&list, &entries[6].entry_node);
	it_i("list_entry_add_tail() should work", (int)list_entry_size(&list),
	     6);

	// get entry
	test_list_entry_t *node =
	    list_entry(&list, &entries[1].entry_node, test_list_entry_t);
	it_b("list_entry() should work", node->data == entries[1].data, true);
	it_b("list_entry_get_first_entry() should work",
	     list_entry_get_first_entry(&list, test_list_entry_t)->data ==
		 entries[1].data,
	     true);
	it_b("list_entry_get_last_entry() should work",
	     list_entry_get_last_entry(&list, test_list_entry_t)->data ==
		 entries[6].data,
	     true);

	it_b("list_entry_size() should work", (int)list_entry_size(&list), 6);

	// list_entry_for_each
	list_entry_t *entry = list.next;
	unsigned int i = 1;
	list_entry_for_each(&list, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
		i++;
	}
	it_b("list_entry_for_each() should work", (i - 1) == 6, true);

	// list_entry_for_each_reverse
	i = 6;
	list_entry_for_each_reverse(&list, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
		i--;
	}
	it_b("list_entry_for_each_reverse() should work",
	     &node->entry_node == list.next && i == 0, true);

	// list_entry_for_each_by_length
	i = 0;
	list_entry_for_each_by_length(&list, i, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
	}
	it_b("list_entry_for_each_by_length() should work",
	     &node->entry_node == list.prev && i == list.length, true);

	// list_entry_for_each_reverse_by_length
	i = 0;
	list_entry_for_each_reverse_by_length(&list, i, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
	}
	it_b("list_entry_for_each_reverse_by_length() should work",
	     &node->entry_node == list.next && i == list.length, true);

	// list_entry_for_each_entry
	i = 0;
	list_entry_for_each_entry(&list, node, entry_node, test_list_entry_t)
	{
		i++;
	}
	it_b("list_entry_for_each_entry() should work", node != NULL && i == 6,
	     true);
	// list_entry_for_each_entry_reverse
	i = 6;
	list_entry_for_each_entry_reverse(&list, node, entry_node,
					  test_list_entry_t)
	{
		i--;
	}
	it_b("list_entry_for_each_entry_reverse() should work",
	     node != NULL && i == 0, true);
	it_b("list_entry_is_empty() should work", !list_entry_is_empty(&list),
	     true);
	it_b("list_is_singular() should work", !list_is_singular(&list), true);

	// repalce entries
	list_entry_replace_head(&list, &entries[0].entry_node);
	list_entry_replace_last(&list, &entries[7].entry_node);
	it_b("list_entry_replace_head() should work",
	     (int)list_entry_size(&list) == 6, true);
	it_b("list_entry_replace_last() should work",
	     (int)list_entry_size(&list) == 6, true);

	// delete entries
	list_entry_delete_head(&list);
	list_entry_delete_last(&list);
	it_i("list_entry_delete_() should work", (int)list_entry_size(&list),
	     4);

	list_entry_t *head = list_entry_get_head(&list);
	list_entry_t *tail = list_entry_get_last(&list);
	it_b("list_entry_get_head() should work",
	     list_entry_is_head(&list, head), true);
	it_b("list_entry_get_last() should work",
	     list_entry_is_last(&list, tail), true);

	list_entry_move_head(&list, tail);
	list_entry_move_tail(&list, head);
	it_b("list_entry_move_head() should work",
	     tail == list_entry_get_head(&list), true);
	it_b("list_entry_move_tail() should work",
	     head == list_entry_get_last(&list), true);

	list_rotate_left(&list);
	it_b("list_rotate_left() should work",
	     tail == list_entry_get_last(&list), true);

	list_entry_head_t new_list;
	list_entry_head_init(&new_list, test_list_entry_t, entry_node);

	list_entry_splice_head(&new_list, &list);
	it_b("list_entry_splice_head() should work",
	     (int)list_entry_size(&list) == 0 && (list.next == list.prev) &&
		 new_list.length != 0,
	     true);
	list_entry_splice_tail(&list, &new_list);
	it_b("list_entry_splice_tail() should work",
	     (int)list_entry_size(&new_list) == 0 &&
		 (new_list.next == new_list.prev) && list.length != 0,
	     true);
	// clear entries
	list_entry_clear(&list);
	it_b("list_entry_clear() should work",
	     (int)list_entry_size(&list) == 0 && (list.next == list.prev),
	     true);

	list_entry_exit(&list);
	it_b("list_entry_exit() should work",
	     (int)list_entry_size(&list) == 0 && !list.next && !list.prev,
	     true);
}
