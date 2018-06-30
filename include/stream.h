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

struct GNStreamInterface
{
	void						(*destroy)	(struct GNStream* stream);
	struct GNStreamLockData*	(*lock)		(struct GNStream* stream, GNIndex start, GNIndex count);
};

struct GNStream
{
	struct GNStreamInterface* i;

	GNType	type;
	GNIndex	count;
};

#endif