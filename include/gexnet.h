#ifndef __GEXNET_H__
#define __GEXNET_H__

#include "types.h"


struct GNSystem
{
	struct _MemoryInterface
	{
		void*(*allocate)(struct GNSystem*, size_t size, size_t flags);
		void(*free)(struct GNSystem*, void* mem);
	}*mem;

	struct GNStream* (*create_stream)(struct GNSystem*, GNType type, GNIndex count, void* data);

	void(*destroy)(struct GNSystem*);
};

struct GNSystem* gexnet_native_init(void* init_data);

#endif