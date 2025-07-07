#ifndef _HUFFMAN_CODE_H_
#define _HUFFMAN_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <limits.h>

#define HC_LHS_BIT (0)
#define HC_RHS_BIT (1)

#define SYMBOLS_COUNT (UCHAR_MAX + 1)

#define hc_node_is_leaf(n) (((n)->lhs == NULL) && ((n)->rhs == NULL))

#define hc_tree_print(root) __hc_tree_print((root), 0, 0, (-1))
#define hc_calc_compressed_size(root, size_ptr) __hc_calc_compressed_size((root), (-1), (size_ptr))

typedef unsigned char hc_byte_t;
typedef hc_byte_t hc_symbol_t;
typedef unsigned long hc_freq_t;

typedef struct hc_node {
	hc_freq_t freq;
	hc_symbol_t symbol;
	struct hc_node *lhs, *rhs, *parent;
} hc_node_t;

typedef struct hc_ctx {
	hc_node_t *nodes, *root;
	size_t base_idx;
} hc_ctx_t;

// typedef struct hc_table {
// 	struct {
// 		hc_freq_t freq;
// 		hc_byte_t code;
// 		hc_byte_t size;
// 	} codes[SYMBOLS_COUNT];
// } hc_table_t;

int hc_ctx_init(hc_ctx_t *ctx);

void hc_ctx_freqs_update(hc_ctx_t *ctx, const hc_symbol_t *syms, size_t n);

int hc_ctx_freqs_finalize(hc_ctx_t *ctx);

int hc_ctx_tree_build(hc_ctx_t *ctx);

void __hc_tree_print(hc_node_t *root, int indent_level,
					unsigned long bits, int bits_count);

void __hc_calc_compressed_size(hc_node_t *root, int bits_count, size_t *size);

void hc_ctx_destroy(hc_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // _HUFFMAN_CODE_H_
