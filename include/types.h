#ifndef __GEXNET_TYPES_H__
#define __GEXNET_TYPES_H__

#include "stddef.h"


typedef float	Number;
typedef long	Integer;
typedef short	Activation_func;

typedef struct
{
	void* (*allocate)(size_t size);
	void  (*free)(void* mem);
}Allocator;

#endif
