#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "arena.h"
#include "huffman_code.h"

#define BUFFER_SIZE ARENA_MB(8ULL)

// These example inputs copy-pasted from wikipedia
// static const hc_byte_t input[] = "this is an example of a huffman tree";
// static const hc_byte_t input[] = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
// static const hc_byte_t input[] = "";

// global memory
static arena_t *gmem;

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

	arena_config_t aconf = ARENA_DEFAULT_CONFIG;
	aconf.reserve = ARENA_MB(64ULL);
	aconf.commit  = BUFFER_SIZE;
	gmem = arena_new(&aconf);
	if ( (err = hc_ctx_init(gmem, &ctx))) {
		fprintf(stderr, "failed to create huffman context\n");
		goto ret;
	}

	fp = fopen(argv[1], "rb");
	if (!fp) {
		perror("fopen");
		goto ret_arena_destroy;
	}

	buffer = arena_alloc(gmem, BUFFER_SIZE);
	if (!buffer) {
		perror("arena_alloc");
		goto ret_close_fp;
	}

	while ( (nread = fread(buffer, sizeof(*buffer), BUFFER_SIZE, fp)) > 0)
		hc_ctx_freqs_update(&ctx, buffer, nread);

	if ( (err = hc_ctx_freqs_finalize(&ctx)))
		goto ret_close_fp;

	if ( (err = hc_ctx_tree_build(gmem, &ctx)))
		goto ret_close_fp;

	hc_tree_print(ctx.root);

	size_t size = 0;
	hc_calc_compressed_size(ctx.root, &size);
	// Shifting the bits 10 times to right is equivalent to divide by 1024
	printf("\nFinal compressed size: %zu KB\n", size >> 10);

ret_close_fp:
	fclose(fp);
ret_arena_destroy:
	arena_destroy(gmem);
ret:
	return err;
}
