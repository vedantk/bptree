#include "bptree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <queue>
using namespace std;

#define MAX(x, y) ((x) > (y) ? (x) : (y))

static int kmax = 1;
static int cwidth = 1;

int bpt_width(struct bptree* bpt)
{
	return 3 + int(bpt->nr_keys) * (cwidth + 2);
}

void bpt_draw_helper(struct bptree* bpt, int lvl, int first)
{
	if (first) {
	    for (int i=0; i < (4-lvl); ++i) printf("\t");
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

void bpt_draw(struct bptree* bpt)
{
    printf("--------------------------------------------------------------\n");
	bpt_draw_bfs(bpt);
	printf("\n");
	bptree_sane(bpt);
}

int main()
{
	struct bptree* bpt = bptree_alloc(0, NULL);
	assert(bptree_lookup(bpt, 0) == NULL || !"Lookup failed.");
	bpt_draw(bpt);

    bptree_insert(&bpt, 5, (void*) 4);
	bpt_draw(bpt);

    bptree_insert(&bpt, 7, (void*) 4);
	bpt_draw(bpt);

    bptree_insert(&bpt, 3, (void*) 4);
	bpt_draw(bpt);

    bptree_insert(&bpt, 6, (void*) 4);
	bpt_draw(bpt);

    bptree_insert(&bpt, 4, (void*) 4);
	bpt_draw(bpt);

	for (uint64_t i=10; i < 19; ++i) {
	    bptree_insert(&bpt, i, (void*) (i * 2));
        bpt_draw(bpt);
	}

	return 0;
}
