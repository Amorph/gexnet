#include "memory.h"
#include <stdlib.h>

allocator default_allocator = { malloc, free };

static allocator* s_allocator = &default_allocator; 

void 		set_allocator(allocator* alloc)
{
	s_allocator = alloc;
}

allocator*	get_allocator()
{
	return s_allocator;
}