#ifndef __GEXNET_NETWORK_H__
#define __GEXNET_NETWORK_H__

#include "types.h"

#define FOURCC_INPUTS				GEXNET_MAKE_FOURCC('I','N',' ',' ')
#define FOURCC_OUTPUTS				GEXNET_MAKE_FOURCC('O','U','T',' ')
#define FOURCC_LINK					GEXNET_MAKE_FOURCC('L','I','N','K')
#define FOURCC_LINK_WEIGHT			GEXNET_MAKE_FOURCC('L','W','G','T')
#define FOURCC_DATA					GEXNET_MAKE_FOURCC('D','A','T','A')
#define FOURCC_BIAS					GEXNET_MAKE_FOURCC('B','I','A','S')
#define FOURCC_ACTIVATION			GEXNET_MAKE_FOURCC('A','C','T','V')
#define FOURCC_FEED_FORWARD_ORDER	GEXNET_MAKE_FOURCC('F','F','O','R')

#define NULL_LINK ((size_t)-1)

typedef struct
{
	size_t	input;
	size_t	output;
} NetworkLink;


typedef struct
{
	FourCC	type;
	size_t	elementSize;
	size_t	elementCount;
	void*	data;
} NetworkStream;

typedef struct
{
	NetworkStream* stream;
	size_t	count;
	void* data;
} NetworkStreamLockData;

typedef struct
{
	size_t			streams_count;
	NetworkStream** streams;
} Network;


Network*				network_create();
void					network_destroy(Network* net);
size_t					network_attach_stream(Network* net, NetworkStream* stream);
NetworkStream*			network_get_stream_type(Network* net, FourCC type);


NetworkStream*			network_stream_create(FourCC type, size_t element_size, size_t count);
void					network_stream_destroy(NetworkStream* stream);

NetworkStreamLockData*	network_stream_lock(NetworkStream* stream, size_t start_element, size_t count);
void					network_stream_unlock(NetworkStreamLockData* lock_data);

#endif