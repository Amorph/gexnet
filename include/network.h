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
	Number 				bias;
	Activation_func		activation_function;
} NetworkNode;

typedef struct
{
	size_t				input;
	size_t				output;
} NetworkLink;


typedef struct
{
	FourCC	type;
	size_t	elementSize;
	Integer	elementCount;
	void*	data;
} NetworkStream;


typedef struct
{
	size_t			streams_count;
	NetworkStream** streams;
} Network;


Network*		network_create();
void			network_destroy(Network* net);
size_t			network_attach_stream(Network* net, NetworkStream* stream);

NetworkStream*	network_stream_create(FourCC type, size_t element_size, Integer count);
void			network_stream_destroy(NetworkStream* stream);

#endif