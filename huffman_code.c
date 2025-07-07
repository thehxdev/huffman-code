#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "malloc.h"
#include "huffman_code.h"

static const hc_node_t sample_nodes[SYMBOLS_COUNT] = {
	// Learned this from tcc source code :)
	#include "sample_nodes.h"
};

static inline int node_freq_comp_fn(const void *l, const void *r) {
	return ((*(hc_freq_t*)l) - (*(hc_freq_t*)r));
}

static inline hc_node_t *hc_nodes_get(hc_node_t **nodes, long count, long idx) {
	if (idx >= 0 && idx < count)
		return nodes[idx];
	return NULL;
}

static inline int hc_nodes_insert(hc_node_t **nodes, long count,
								  const hc_node_t *n, long base_idx)
{
	long i;
	for (i = base_idx; i < count && nodes[i]->freq < n->freq; i++);
	memmove(&nodes[base_idx-1], &nodes[base_idx], sizeof(*nodes) * (i - base_idx));
	nodes[i-1] = (hc_node_t*)n;
	return 0;
}

int hc_ctx_init(hc_ctx_t *ctx) {
	ctx->nodes = malloc(sizeof(*ctx->nodes) * SYMBOLS_COUNT);
	if (!ctx->nodes)
		return 1;

	memmove(ctx->nodes, sample_nodes, sizeof(sample_nodes));
	return 0;
}

void hc_ctx_freqs_update(hc_ctx_t *ctx, const hc_symbol_t *syms, size_t n) {
	size_t i;
	hc_node_t *nodes = ctx->nodes;
	for (i = 0; i < n; i++)
		nodes[(syms[i])].freq += 1;
}

int hc_ctx_freqs_finalize(hc_ctx_t *ctx) {
	size_t i;
	qsort(ctx->nodes, SYMBOLS_COUNT, sizeof(*ctx->nodes), node_freq_comp_fn);

	for (i = 0; ctx->nodes[i].freq == 0; i++);
	ctx->base_idx = i;

	return 0;
}

int hc_ctx_tree_build(hc_ctx_t *ctx) {
	int err = 0;
	long lidx, ridx;
	hc_node_t *lhs, *rhs, *tmp = NULL, *nodes_addr[SYMBOLS_COUNT];
	const long nodes_count = SYMBOLS_COUNT - ctx->base_idx;

	for (lidx = 0, ridx = ctx->base_idx; ridx < SYMBOLS_COUNT; lidx++, ridx++)
		nodes_addr[lidx] = &ctx->nodes[ridx];

	lidx = 0;
	ridx = 1;

	while (1) {
		lhs = hc_nodes_get(nodes_addr, nodes_count, lidx);
		if (lhs != NULL)
			lidx++;

		rhs = hc_nodes_get(nodes_addr, nodes_count, ridx);
		if (rhs != NULL)
			ridx++;

		if (lhs == NULL && rhs == NULL)
			break;

		// Create the parent node then set lhs and rhs
		tmp = malloc(sizeof(*tmp));
		if (!tmp) {
			err = 1;
			goto ret;
		}
		memset(tmp, 0, sizeof(*tmp));
		tmp->lhs = lhs;
		tmp->rhs = rhs;

		if (lhs) {
			lhs->parent = tmp;
			tmp->freq += lhs->freq;
		}
		if (rhs) {
			rhs->parent = tmp;
			tmp->freq += rhs->freq;
		}

		hc_nodes_insert(nodes_addr, nodes_count, tmp, lidx+1);
	}

	ctx->root = tmp;
ret:
	return err;
}

// Traverse the tree and print each symbol's info
// F***ing recursion! It's 5 AM already. F*** you recursion.
void __hc_tree_print(hc_node_t *root, int indent_level,
						unsigned long bits, int bits_count)
{
	int i;
	if (!root)
		return;
	if (hc_node_is_leaf(root)) {
		while (indent_level--)
			putchar(' ');
		printf(" 0x%.2x = { freq = %ld, code = ", root->symbol, root->freq);
		for (i = bits_count-1; i >= 0; i--)
			putchar(((bits >> i) & 0x1) + 48);
		printf(", len = %2d }\n", bits_count);
	} else {
		__hc_tree_print(root->lhs, indent_level + 1, (bits << 1), bits_count+1);
		__hc_tree_print(root->rhs, indent_level + 1, (bits << 1) | 1, bits_count+1);
	}
}

void __hc_calc_compressed_size(hc_node_t *root, int bits_count, size_t *size) {
	if (!root)
		return;
	if (hc_node_is_leaf(root)) {
		// Shifting the bits 3 times to right is equivalent to divide by 8
		*size += (root->freq * bits_count) >> 3;
	} else {
		__hc_calc_compressed_size(root->lhs, bits_count+1, size);
		__hc_calc_compressed_size(root->rhs, bits_count+1, size);
	}
}

static void hc_tree_destroy(hc_node_t *root) {
	if (!root)
		return;
	hc_node_t *rhs = root->rhs;
	hc_tree_destroy(root->lhs);
	if (!hc_node_is_leaf(root))
		free(root);
	hc_tree_destroy(rhs);
}

void hc_ctx_destroy(hc_ctx_t *ctx) {
	hc_tree_destroy(ctx->root);
	free(ctx->nodes);
}
