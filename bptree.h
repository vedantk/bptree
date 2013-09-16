/*
 * Copyright (c) 2013 Vedant Kumar <vsk@berkeley.edu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.  THE SOFTWARE IS
 * PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#ifdef __cplusplus__
extern "C" {
#endif

#include <stdint.h>

#define ORDER 4

struct bptree {
	uint64_t* keys;
	void** pointers;
	uint16_t is_leaf : 1;
	uint16_t nr_keys : 15;
} __attribute__((packed));

/* Create a tree with an initial tuple. */
struct bptree* bptree_alloc(uint64_t key, void* val);

/* Find the leaf containing a key, or return NULL. */
struct bptree* bptree_exists(struct bptree* bpt, uint64_t key);

/* Update the value of a key in the given subtree. */
void bptree_modify(struct bptree* bpt, uint64_t key, void* val);

/* Insert a new tuple into the tree (with a unique key). */
void bptree_insert(struct bptree** bpt, uint64_t key, void* val);

/* Lookup the value corresponding to a key (NULL if nonexistent). */
void* bptree_lookup(struct bptree* bpt, uint64_t key);

/* Find the closest leaf node to a key. */
struct bptree* bptree_search(struct bptree* bpt, uint64_t key);

/* Find the leaf node following a given node. */
struct bptree* bptree_next(struct bptree* bpt);

/* Delete a tuple from the tree, returning its associated value. */
void* bptree_delete(struct bptree** bpt, uint64_t key);

/* Destroy the tree. */
void bptree_free(struct bptree* bpt);

#ifdef __cplusplus__
} /* extern "C" */
#endif
