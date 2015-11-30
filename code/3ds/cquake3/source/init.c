#include <sys/iosupport.h>
#include <sys/time.h>

#include <string.h>
#include <3ds/types.h>
#include <3ds/svc.h>

#if 1
u32 __stacksize__ = 1024 * 1024;

extern char* fake_heap_start;
extern char* fake_heap_end;
extern u32 __heap_size, __linear_heap_size;
extern u32 __linear_heap;
extern u32 __heapBase;

#if 1
void __system_allocateHeaps() {
	u32 total_size;
	u32 tmp = 0;

	//increase linear size for hax 2.5
	total_size = __heap_size + __linear_heap_size;
	if (total_size > (40 * 1024 * 1024)) {
		//__linear_heap_size = 32 * 1024 * 1024;
		//__heap_size = total_size - __linear_heap_size;// -(128 * 1024);
	}

	// Allocate the application heap
	__heapBase = 0x08000000;
	svcControlMemory(&tmp, __heapBase, 0x0, __heap_size, MEMOP_ALLOC, MEMPERM_READ | MEMPERM_WRITE);

	// Allocate the linear heap
	svcControlMemory(&__linear_heap, 0x0, 0x0, __linear_heap_size, MEMOP_ALLOC_LINEAR, MEMPERM_READ | MEMPERM_WRITE);
	// Set up newlib heap
	fake_heap_start = (char*)__heapBase;
	fake_heap_end = fake_heap_start + __heap_size;

}
#endif

#endif
