#include "bptree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include <queue>
using namespace std;

#define MAGIC ((void*) 0x42)
#define VALUE(x) ((void*) (x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define bpt_next pointers[0]

#define BPT_P(bpt, pidx) ((struct bptree*) (bpt)->pointers[(pidx)])
#define BPT_PREF(bpt, pidx) ((struct bptree**) &((*bpt)->pointers[(pidx)]))

void bpt_draw_helper(struct bptree* bpt, int lvl, int first)
{
	if (first) {
	    for (int i=0; i < (5-lvl); ++i) printf("\t");
	}

	printf("[");
	for (int i=0; i < bpt->nr_keys; ++i) {
		printf(" %llu ", bpt->keys[i]);
	}
	printf("] ");
}

void bpt_draw_bfs(struct bptree* root)
{
    int lvl = 1;
    queue<struct bptree*> visited;
    queue<struct bptree*> this_lvl;
    this_lvl.push(root);

    while (!this_lvl.empty()) {
        struct bptree* bpt = this_lvl.front();
        this_lvl.pop();

        bpt_draw_helper(bpt, lvl, visited.empty());
        visited.push(bpt);

        if (this_lvl.empty()) {
            ++lvl;
            printf("\n");

            if (!bpt->is_leaf) {
                while (!visited.empty()) {
                    struct bptree* child = visited.front();
                    visited.pop();

                    for (int i=0; i <= child->nr_keys; ++i) {
                        if (child->pointers[i]) {
                            this_lvl.push((struct bptree*) child->pointers[i]);
                        }
                    }
                }
            }
        }
    }
}

inline int split(int x)
{
	return (x >> 1) + (x & 1);
}

void bptree_sane(struct bptree* bpt, int root)
{
	for (int i=1; i < bpt->nr_keys; ++i) {
		assert(bpt->keys[i] > bpt->keys[i-1]);
	}
	if (!bpt->is_leaf) {
		for (int i=0; i <= bpt->nr_keys; ++i) {
			if (BPT_P(bpt, i))
				bptree_sane(BPT_P(bpt, i), 0);
		}
		for (int i=1; i <= bpt->nr_keys; ++i) {
			if (BPT_P(bpt, i) && BPT_P(bpt, i-1))
				assert(BPT_P(bpt, i)->keys[0] > 
					BPT_P(bpt, i-1)->keys[BPT_P(bpt, i-1)->nr_keys - 1]);
		}
	}

	if (!root) {
		assert(bpt->nr_keys >= split(ORDER) - 1);
	}
}

void bpt_draw(struct bptree* bpt)
{
	bpt_draw_bfs(bpt);
	printf("\n");
	bptree_sane(bpt, true);
}

void test_inserts()
{
    struct bptree* bpt = bptree_alloc(0, NULL);
    assert(bpt);
    assert(bptree_lookup(bpt, 0) == NULL);

    // Test sequential inserts.
    for (uint64_t k=0; k < 30; ++k) {
        bptree_insert(&bpt, k, VALUE(k + 0x42));
    }
    for (uint64_t k=1; k < 30; ++k) {
        assert(bptree_lookup(bpt, k) == VALUE(k + 0x42));
    }
    bptree_sane(bpt, 1);

    // Inserts should not overwrite existing values.
    assert(bptree_lookup(bpt, 0) == NULL);
    for (uint64_t k=0; k < 30; ++k) {
        bptree_insert(&bpt, k, NULL);
    }
    for (uint64_t k=1; k < 30; ++k) {
        assert(bptree_lookup(bpt, k) == VALUE(k + 0x42));
    }
    bptree_sane(bpt, 1);

    // Test random inserts.
    for (int i=0; i < 10000; ++i) {
        uint64_t key = rand() % 10000;
        if (!bptree_exists(bpt, key)) {
            bptree_insert(&bpt, key, VALUE(key + 1));
            assert(bptree_lookup(bpt, key) == VALUE(key + 1));
        }
    }
    bptree_sane(bpt, 1);
    bptree_free(bpt);
}

void test_modify()
{
    struct bptree* bpt = bptree_alloc(0, MAGIC);
    assert(bptree_lookup(bpt, 0) == MAGIC);

    // Sequentially insert tuples.
    for (uint64_t i=1; i < 10000; ++i) {
        bptree_insert(&bpt, i, MAGIC);
    }
    for (uint64_t i=0; i < 10000; ++i) {
        struct bptree* leaf = bptree_exists(bpt, i);
        assert(leaf);
        bptree_modify(leaf, i, VALUE(i));
        assert(bptree_lookup(leaf, i) == VALUE(i));
        assert(bptree_lookup(bpt, i) == VALUE(i));
    }
    bptree_sane(bpt, 1);

    // Test random updates.
    for (int i=0; i < 10000; ++i) {
        uint64_t key = rand() % 10000;
        void* val = VALUE(rand());
        bptree_modify(bpt, key, val);
        assert(bptree_lookup(bptree_exists(bpt, key), key) == val);
    }
    bptree_sane(bpt, 1);
    bptree_free(bpt);
}

void test_iterate()
{
    struct bptree* bpt = bptree_alloc(0, NULL);
    
    // Seed the tree with sequential inserts.
    for (uint64_t i=1; i < 10000; ++i) {
        bptree_insert(&bpt, i, VALUE(i));
    }

    // Start in random locations and scan to the end of the tree.
    for (int i=0; i < 100; ++i) {
        uint64_t key = rand() % 10000;
        struct bptree* leaf = bptree_search(bpt, key);
        assert(bptree_exists(bpt, key) == leaf);
        while (1) {
            for (int k=1; k < leaf->nr_keys; ++k) {
                assert(leaf->keys[k] == leaf->keys[k-1] + 1);
            }
            if (bptree_next(leaf)) {
                leaf = bptree_next(leaf);
            } else {
                break;
            }
        }
        assert(leaf->keys[leaf->nr_keys - 1] == (10000 - 1));
    }
    bptree_free(bpt);
}

void test_deletes()
{
    struct bptree* bpt = bptree_alloc(10, MAGIC);
    assert(ORDER == 4 && "These tests were designed for ORDER=4.");
    bptree_insert(&bpt, 20, MAGIC);
    bptree_insert(&bpt, 5, MAGIC);
    bptree_insert(&bpt, 7, MAGIC);
    bptree_insert(&bpt, 9, MAGIC);
    bptree_insert(&bpt, 13, MAGIC);
    bptree_insert(&bpt, 22, MAGIC);
    bptree_insert(&bpt, 12, MAGIC);
    bpt_draw(bpt);

    bptree_delete(&bpt, 10); bpt_draw(bpt);
    bptree_delete(&bpt, 12); bpt_draw(bpt);
    bptree_delete(&bpt, 9); bpt_draw(bpt);
    bptree_delete(&bpt, 13); bpt_draw(bpt);
    bptree_delete(&bpt, 20); bpt_draw(bpt);
    bptree_delete(&bpt, 22); bpt_draw(bpt);
    bptree_delete(&bpt, 5); bpt_draw(bpt);
    bptree_delete(&bpt, 5); bpt_draw(bpt);
    bptree_delete(&bpt, 7); bpt_draw(bpt);

    bptree_free(bpt);
}

void test_insert_delete_iterate()
{

}

int main()
{
    srand(time(NULL));

#if 0
    printf("test_inserts...\n");
    test_inserts();

    printf("test_modify...\n");
    test_modify();

    printf("test_iterate...\n");
    test_iterate();
#endif

    printf("test_deletes...\n");
    test_deletes();

    printf("test_insert_delete_iterate...\n");
    test_insert_delete_iterate();

    return 0;
}
