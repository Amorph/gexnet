#ifndef __GEXNET_STREAM_H__
#define __GEXNET_STREAM_H__

#include "types.h"

#define GN_FUNCTION_TANH (0)


struct GNStreamLockData
{
	GNType				type;
	void*				data;
	GNIndex				count;

	struct GNStream*	stream;
};


struct GNStream
{
	GNType	type;
	GNIndex	count;

	struct GNSystem* system;
};

#endif