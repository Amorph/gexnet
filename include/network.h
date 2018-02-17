#ifndef __GEXNET_NETWORK_H__
#define __GEXNET_NETWORK_H__

#include "types.h"

typedef struct 
{
	number 				bias;
	activation_func		activation_function;
} network_node;

typedef struct
{
	network_node*		input;
	network_node*		output;
	number		 		weight;
} network_link;

typedef struct
{
	number				value;
} network_node_data;

typedef struct 
{
	integer				node_count;
	integer				links_count;

	network_node*		nodes;
	network_link*		links;
} network;

typedef struct
{
	integer				node_count;
	network_node_data*	node_data;
} network_data;

network*		create_network(integer node_count, integer links_count);
network_data*	create_network_data(integer node_count);

void			destroy_network(network* net);
void			destroy_network_data(network_data* data);


#endif