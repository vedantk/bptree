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

#include <stdint.h>

#define ORDER 256

#ifdef __cplusplus__
extern "C" {
#endif

struct bptree {
	uint64_t* keys;
	void** pointers;
	uint16_t is_leaf : 1;
	uint16_t nr_keys : 15;
} __attribute__((packed));

struct bptree* bptree_alloc(uint64_t key, void* val);
struct bptree* bptree_search(struct bptree* bpt, uint64_t key);
struct bptree* bptree_next(struct bptree* bpt);
void* bptree_lookup(struct bptree* bpt, uint64_t key);
void bptree_insert(struct bptree** bpt, uint64_t key, void* val);
void* bptree_delete(struct bptree** bpt, uint64_t key);
void bptree_free(struct bptree* bpt);

#ifdef __cplusplus__
} /* extern "C" */
#endif
