#include "bptree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <queue>
using namespace std;

#define MAX(x, y) ((x) > (y) ? (x) : (y))

static int cwidth = 1;

int bpt_width(struct bptree* bpt)
{
	return 3 + bpt->nr_keys * (cwidth + 2);
}

void bpt_draw_helper(struct bptree* bpt, int leaves_width, int lvl, int first)
{
	if (first) {
		int spaces = (leaves_width / (lvl + 1)) - (bpt_width(bpt) / 2);
		for (int i=0; i < spaces; ++i) printf(" ");
	}

	printf("[");
	for (int i=0; i < bpt->nr_keys; ++i) {
		printf(" %llu ", bpt->keys[i]);
	}
	printf("] ");
}


void bpt_draw_bfs(queue<struct bptree*>& q, int leaves_width, int lvl)
{
    int visited = 0;
    while (!q.empty()) {
        struct bptree* bpt = q.front();
        q.pop();

        for (int i=0; i < bpt->nr_keys; ++i) {
            bpt_draw_helper(bpt, leaves_width, lvl, visited == 0);
            ++visited;
        }

        if (ORDER % visited == 0) {
            visited = 0;
            ++lvl;
            printf("\n");
        }

        if (!bpt->is_leaf) {
            for (int i=0; i <= bpt->nr_keys; ++i) {
                q.push((struct bptree*) bpt->pointers[i]);
            }
        }
    }
}

void bpt_draw(struct bptree* bpt)
{
	uint64_t n = 0;
	uint64_t kmax = 0;
	uint64_t leaves_width = 0;
	struct bptree* orig = bpt;
	while ((bpt = bptree_next(bpt))) {
		for (int i=0; i < bpt->nr_keys; ++i) {
			++n;
			kmax = MAX(kmax, bpt->keys[i]);
		}

		cwidth = log(kmax) / log(10);
		leaves_width += bpt_width(bpt);
	}

    queue<struct bptree*> q;
    q.push(orig);
	bpt_draw_bfs(q, leaves_width, 1);
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

	return 0;
}
