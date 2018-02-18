#ifndef __GEXNET_NETWORK_H__
#define __GEXNET_NETWORK_H__

#include "types.h"

typedef struct 
{
	Number 				bias;
	Activation_func		activation_function;
} NetworkNode;

typedef struct
{
	NetworkNode*		input;
	NetworkNode*		output;
	Number		 		weight;
} NetworkLink;

typedef struct
{
	Number				value;
} NetworkNodeData;

typedef struct 
{
	Integer				node_count;
	Integer				links_count;

	NetworkNode*		nodes;
	NetworkLink*		links;
} Network;

typedef struct
{
	Integer				node_count;
	NetworkNodeData*	node_data;
} NetworkData;


Network*		network_create(Integer node_count, Integer links_count);
NetworkData*	network_create_data(Integer node_count);

void			network_destroy(Network* net);
void			network_destroy_data(NetworkData* data);


#endif