#ifndef __GEXNET_TYPES_H__
#define __GEXNET_TYPES_H__

#include "stddef.h"
#include "stdint.h"
#include <stdbool.h>

typedef float		Number;
typedef long		Integer;
typedef short		Activation_func;
typedef uint32_t	FourCC;


typedef float				GNNumber;
typedef size_t				GNIndex;
typedef long				GNInteger;
typedef uint32_t			GNType;

#define GN_TYPE_UNKNOWN		(0)
#define GN_TYPE_FLOAT		(1) // float or double float or half float :)
#define GN_TYPE_INTEGER		(2) // signed number
#define GN_TYPE_INDEX		(3) // unsigned number
#define GN_TYPE_INDEX_INDEX	(4)

#define GN_TYPE_LINK		GN_TYPE_INDEX_INDEX
#define GN_TYPE_NUMBER		GN_TYPE_FLOAT


typedef struct XYZIntVector
{
	Integer x, y, z;
}XYZIntVector;

typedef struct
{
	void* (*allocate)(size_t size);
	void  (*free)(void* mem);
}Allocator;

typedef struct
{
	GNIndex	input;
	GNIndex	output;
} GNLink;


#define GEXNET_MAKE_FOURCC(ch0, ch1, ch2, ch3) \
    ((uint32_t)(uint8_t)(ch0) | \
    ((uint32_t)(uint8_t)(ch1) << 8) | \
    ((uint32_t)(uint8_t)(ch2) << 16) | \
    ((uint32_t)(uint8_t)(ch3) << 24))


#endif
