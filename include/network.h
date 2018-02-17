#ifndef __GEXNET_NETWORK_H__
#define __GEXNET_NETWORK_H__

#include "types.h"

typedef struct 
{
	number 			bias;
	activation_func	activation_function;
} network_node;

typedef struct
{
	network_node*	input;
	network_node*	output;
	number		 	weight;
} network_link;

typedef struct node_data
{
	number			value;
} network_data;

typedef struct 
{
	integer			node_count;
	integer			links_count;

	network_node*	nodes;
	network_link*	links;
} network;

network*	create_network(integer node_count, integer links_count);
number*		create_network_data(network* net);

void		destroy_network(network* net);
void		destroy_network_data(number* data);


#endif