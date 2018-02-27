#ifndef __GEXNET_TYPES_H__
#define __GEXNET_TYPES_H__

#include "stddef.h"
#include "stdint.h"
#include <stdbool.h>

typedef float		Number;
typedef long		Integer;
typedef short		Activation_func;
typedef uint32_t	FourCC;

typedef struct
{
	void* (*allocate)(size_t size);
	void  (*free)(void* mem);
}Allocator;

typedef enum
{
	NPT_LINK,
	NPT_NODE
} NetworkPassType;

#define GEXNET_MAKE_FOURCC(ch0, ch1, ch2, ch3) \
    ((uint32_t)(uint8_t)(ch0) | \
    ((uint32_t)(uint8_t)(ch1) << 8) | \
    ((uint32_t)(uint8_t)(ch2) << 16) | \
    ((uint32_t)(uint8_t)(ch3) << 24))

#endif
