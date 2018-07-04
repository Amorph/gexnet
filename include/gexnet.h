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

	struct _StreamInterface
	{
		void(*destroy)(struct GNStream* stream);
		struct GNStreamLockData*(*lock)(struct GNStream* stream, GNIndex start, GNIndex count, size_t flags);
		void(*unlock)		(struct GNStreamLockData* lck);

		// stream[i] = 0
		void(*clear)(struct GNStream*);
		// stream[i] = input[i]
		void(*copy)(struct GNStream*, struct GNStream* input);
		// stream[i] = data[i]
		void(*set_stream_data)(struct GNStream*, void* data, size_t count);
		// stream[indexes[i]] = data[i]
		void(*set_stream_data_indexed)(struct GNStream*, struct GNStream* indexes, void* data);
		// output[i] = input[indexes[i]]
		void(*get_stream_data_indexed)(struct GNStream*, struct GNStream* indexes, struct GNStream* output);
		// stream[links[i][1]] += x[links[i][0]] * weights[i]
		void(*multiply_add_links)(struct GNStream*, struct GNStream* links, struct GNStream* x, struct GNStream* weights);
		// stream[i] += input[i]
		void(*add)(struct GNStream*, struct GNStream* input);
		// stream[i] = function(input[i])
		void(*process_stream)(struct GNStream*, struct GNStream* input, size_t function);
	}*stream;

	struct GNStream* (*create_stream)(struct GNSystem*, GNType type, GNIndex count, void* data);

	void(*destroy)(struct GNSystem*);
};

struct GNSystem* gexnet_native_init(void* init_data);

#endif