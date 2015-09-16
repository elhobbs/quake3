#include "ctr_rend.h"
#include <stdlib.h>
#include <3ds/linear.h>

#define CTR_REND_BUFFER_MAX (2 * 1024 * 1024)

void ctr_rend_buffer_init() {
	ctr_state.buffer = linearAlloc(CTR_REND_BUFFER_MAX);
	ctr_state.buffer_len = CTR_REND_BUFFER_MAX;
	ctr_state.buffer_pos = 0;
}

void* ctr_rend_buffer_alloc(int len) {
	int cb = (len + 15) & (~15);
	if (ctr_state.buffer_pos + cb > CTR_REND_BUFFER_MAX) {
		return 0;
	}
	GLubyte *addr = &ctr_state.buffer[ctr_state.buffer_pos];
	ctr_state.buffer_pos += cb;
	return addr;
}

void* ctr_rend_buffer_copy(void *p, int len) {
	int cb = (len + 15) & ( ~15 );
	if (ctr_state.buffer_pos + cb > CTR_REND_BUFFER_MAX) {
		printf("-------------\n\nOut of Memory\n\n-------------\n");
		return 0;
	}
	GLubyte *addr = &ctr_state.buffer[ctr_state.buffer_pos];
	memcpy(addr, p, len);
	ctr_state.buffer_pos += cb;

	return addr;
}

void* ctr_rend_buffer_copy_stride(void *p, int count, int size, int stride) {
	int len = size * count;
	int cb = (len + 15) & (~15);
	if (ctr_state.buffer_pos + cb > CTR_REND_BUFFER_MAX) {
		printf("-------------\n\nOut of Memory\n\n-------------\n");
		return 0;
	}
	GLubyte *addr = &ctr_state.buffer[ctr_state.buffer_pos];
	u8 *src = p;
	u8 *dst = addr;
	int i;
	for (i = 0; i < count; i++) {
		memcpy(dst, src, size);
		src += stride;
		dst += size;
	}
	ctr_state.buffer_pos += cb;

	return addr;
}

void ctr_rend_buffer_reset() {
	ctr_state.buffer_pos = 0;
}