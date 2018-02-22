#ifndef __GEXNET_NETWORK_H__
#define __GEXNET_NETWORK_H__

#include "types.h"

//     Network -> Node -> bias, activation
//             -> Link -> input, output, weight
//
//		Network process in two steps:
//			1. Process links by formula output += input * weight
//			2. Apply bias and activation function: node_data = activation_func(node_data + bias)

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