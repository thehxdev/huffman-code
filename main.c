#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sth/string.h>

#include "malloc.h"
#include "huffman_code.h"

#define BUFFER_SIZE (1024 * 1024)

// These example inputs copy-pasted from wikipedia
// static const hc_byte_t input[] = "this is an example of a huffman tree";
// static const hc_byte_t input[] = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
// static const hc_byte_t input[] = "";

int main(int argc, char *argv[]) {
	int err = 0;
	FILE *fp = NULL;
	hc_ctx_t ctx;
	size_t nread;
	unsigned char *buffer = NULL;

	if (argc != 2) {
		printf("Usage: %s <file-path>\n", argv[0]);
		exit(1);
	}

	// use sth memory debugger and allocation tracer to find memory bugs
	// and leaks.
	sth_mem_dbg_init();

	if ( (err = hc_ctx_init(&ctx))) {
		fprintf(stderr, "failed to create huffman context\n");
		goto ret;
	}

	fp = fopen(argv[1], "rb");
	if (!fp) {
		perror("fopen");
		goto destroy_ctx_ret;
	}

	buffer = malloc(BUFFER_SIZE);
	if (!buffer) {
		perror("malloc");
		goto close_fp_ret;
	}

	while ( (nread = fread(buffer, sizeof(*buffer), BUFFER_SIZE, fp)) > 0)
		hc_ctx_freqs_update(&ctx, buffer, nread);

	if ( (err = hc_ctx_freqs_finalize(&ctx)))
		goto free_buffer_ret;

	if ( (err = hc_ctx_tree_build(&ctx)))
		goto free_buffer_ret;

	hc_ctx_tree_print(ctx.root);

	// Just a trap for debugging
	// puts("trap!");
	// *(volatile char*)0;

// Defer-like return
free_buffer_ret:
	free(buffer);
close_fp_ret:
	if (fp)
		fclose(fp);
destroy_ctx_ret:
	hc_ctx_destroy(&ctx);
ret:
	sth_mem_dbg_print();
	sth_mem_dbg_destroy();
	return err;
}
