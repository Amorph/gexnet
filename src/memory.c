#include "memory.h"
#include <stdlib.h>

Allocator default_allocator = { malloc, free };

static Allocator* s_allocator = &default_allocator; 

void allocator_set(Allocator* alloc)
{
	s_allocator = alloc;
}

Allocator* allocator_get()
{
	return s_allocator;
}