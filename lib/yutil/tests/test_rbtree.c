#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/rbtree.h"
#include "yutil/time.h"

static const int count = 1 << 20;

void test_rbtree(void)
{
        int i;
        int c = 0;
        const int delete_count = 1024;
        const int search_count = 1024;
        rbtree_t tree = { 0 };
        rbtree_node_t *s, *node;
        rbtree_init(&tree);

        char *v[] = {
                "apple", "banana", "cherry",    "grape",      "lemon",
                "mango", "pear",   "pineapple", "strawberry", "watermelon"
        };

        long long time1 = (long long)y_gettime();
        for (i = 0; i < count; i++) {
                rbtree_insert(&tree, i + 1, NULL, v[i % 10]);
        }
        printf("Inserting %d nodes spends %lld ms.\n", count,
               (long long)(y_gettime() - time1));
        it_i("rbtree_insert() should work", tree.total_node, count);

        c = 0;

        srand((unsigned int)time(0));
        long long time2 = (long long)y_gettime();
        for (i = 0; i < search_count; i++) {
                s = rbtree_search_by_key(&tree, (rand() % count) + 1);
                if (s) {
                        c++;
                }
        }

        printf("Searching %d nodes among %d spends %lld ms.\n", search_count,
               count, (long long)(y_gettime() - time2));
        it_i("rbtree_search_by_key() should work", c, search_count);

        long long time3 = (long long)y_gettime();
        for (i = 1; i <= delete_count; i++) {
                rbtree_delete(&tree, i, NULL);
        }

        printf("Deleting %d nodes among %d spends %lld ms.\n", delete_count,
               count, (long long)(y_gettime() - time3));
        it_i("rbtree_delete() should work", tree.total_node,
             count - delete_count);

        long long time4 = (long long)y_gettime();
        rbtree_destroy(&tree);
        printf("Destroy %d nodes among %d spends %lld ms.\n", count, count,
               (long long)(y_gettime() - time4));
        c = 0;
        for (node = rbtree_get_min(tree.root); node; node = rbtree_next(node)) {
                c++;
        }
        it_i("rbtree_destroy() should work", c, 0);
}
