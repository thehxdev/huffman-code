#ifndef _HUFFMAN_CODE_MALLOC_H_
#define _HUFFMAN_CODE_MALLOC_H_

// Global malloc header file to choose default memory allocator between STH
// provided one (by default MiMalloc) and libc malloc.

#define STH_MALLOC
#ifdef STH_MALLOC
	// memory allocation tracer provided by STH
	// #define STH_MEM_DBG
	#include <sth/malloc.h>

	#define malloc(s) sth_malloc(s)
	#define calloc(c, s) sth_calloc(c, s)
	#define realloc(p, s) sth_realloc(p, s)
	#define free(p) sth_free(p)

	#define strndup(s, n) sth_strndup(s, n)
	#define strdup(s) sth_strdup(s)

	#define strndup_fast(s, n) sth_strndup_fast(s, n)
	#define strdup_fast(s) sth_strdup_fast(s)
#else
	#include <stdlib.h>
#endif // STH_MALLOC

#endif // _HUFFMAN_CODE_MALLOC_H_
