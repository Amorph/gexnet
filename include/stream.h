#ifndef __GEXNET_STREAM_H__
#define __GEXNET_STREAM_H__

#include "types.h"



struct GNStreamLockData
{
	void(*unlock)	(struct GNStreamLockData* lck);
	GNType			type;
	void*			data;
	GNIndex			count;
};


struct GNStream
{
	struct _GNStreamInterface
	{
		void						(*destroy)	(struct GNStream* stream);
		struct GNStreamLockData*	(*lock)		(struct GNStream* stream, GNIndex start, GNIndex count, size_t flags);

		// stream[i] = data[i]
		void(*set_stream_data)(void* data, size_t count);
		// stream[indexes[i]] = data[i]
		void(*set_stream_data_indexed)(struct GNStream* indexes, void* data);
		// stream[i] = input[indexes[i]]
		void(*get_stream_data_indexed)(struct GNStream* indexes, struct GNStream* input);
		// stream[links[i][1]] += x[links[i][0]] * weights[i]
		void(*multiply_add_links)(struct GNStream* links, struct GNStream* x, struct GNStream* weights);
		// stream[i] = function(input[i])
		void(*process_stream)(struct GNStream* input, size_t function);
	}*i;

	GNType	type;
	GNIndex	count;
};

#endif