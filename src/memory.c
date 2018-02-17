#include "memory.h"

static allocator* s_allocator = 0; 

void 		set_allocator(allocator* alloc)
{
	s_allocator = alloc;
}

allocator*	get_allocator()
{
	return s_allocator;
}