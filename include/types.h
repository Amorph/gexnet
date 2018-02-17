#ifndef __GEXNET_TYPES_H__
#define __GEXNET_TYPES_H__

#include "stddef.h"


typedef float	number;
typedef long	integer;
typedef short	activation_func;

typedef struct
{
	void* (*allocate)(size_t size);
	void  (*free)(void* mem);
}allocator;

#endif
