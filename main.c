#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sth/string.h>

#include "malloc.h"
#include "huffman_code.h"

// These example inputs copy-pasted from wikipedia
// static const hc_byte_t input[] = "this is an example of a huffman tree";
static const hc_byte_t input[] = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
// static const hc_byte_t input[] = "";

int main(void) {
	// use sth memory debugger and allocation tracer to find memory bugs
	// and leaks.
	sth_mem_dbg_init();

	int err;
	hc_ctx_t ctx;

	const size_t input_size = sizeof(input)-1;

	err = hc_ctx_init(&ctx);
	assert(err == 0);

	hc_ctx_freqs_update(&ctx, input, input_size);
	err = hc_ctx_freqs_finalize(&ctx);
	assert(err == 0);

	err = hc_ctx_tree_build(&ctx);
	assert(err == 0);

	hc_ctx_tree_print(ctx.root);

	// puts("trap!");
	// *(volatile char*)0;

	hc_ctx_destroy(&ctx);

	sth_mem_dbg_print();
	sth_mem_dbg_destroy();
	return 0;
}
