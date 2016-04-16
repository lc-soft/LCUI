#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <stdio.h>

//#define DEBUG
#ifdef DEBUG
static struct {
	int alloc_count;
	int free_count;
	int saved_count;
	int alloc_total_size;
	int free_total_size;
	int saved_total_size;
} mem_debug = { 0, 0, 0, 0, 0, 0 };

void *malloc( size_t size )
{
	int *head;
	mem_debug.saved_total_size += size;
	if( size > 32 ) {
		_DEBUG_MSG( "malloc[%d]: %lu bytes, saved_total: %d bytes\n", mem_debug.saved_count, size, mem_debug.saved_total_size );
	}
	head = (int*)malloc(size+sizeof(int));
	*head = size;
	++mem_debug.saved_count;
	++mem_debug.alloc_count;
	mem_debug.alloc_total_size += size; 
	return (void*)(head+1);
}

void free( void *p )
{
	int *head;
	head = p;
	head -= 1;
	++mem_debug.free_count;
	mem_debug.saved_count = mem_debug.alloc_count - mem_debug.free_count; 
	mem_debug.free_total_size += *head; 
	mem_debug.saved_total_size = mem_debug.alloc_total_size - mem_debug.free_total_size; 
	if( *head > 32 ) {
		_DEBUG_MSG( "free[%d]: %d bytes, saved_total: %d bytes\n", mem_debug.saved_count, *head, mem_debug.saved_total_size );
	}
	free(head);
}
#endif
