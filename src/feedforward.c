#include "feedforward.h"
#include "memory.h"
#include <string.h>

FFNetwork* ff_network_create(Integer layers_count)
{
	FFNetwork* net = allocator_get()->allocate(sizeof(FFNetwork));

	net->layers_count = layers_count;
	net->layers = allocator_get()->allocate(sizeof(net->layers) * layers_count);
	memset(net->layers, 0, sizeof(net->layers) * layers_count);

	return net;
}

void ff_network_destroy(FFNetwork * network)
{
	for (int i = 0; i < network->layers_count; i++)
	{
		if (network->layers[i])
			network_destroy(network->layers[i]);
	}
	allocator_get()->free(network);
}

void ff_network_init_layer(FFNetwork* network, Integer layer, Integer nodes, Integer links)
{
	network->layers[layer] = network_create(nodes, links);
}

void ff_network_init_fully_connected_layer(FFNetwork* network, Integer layer, Integer nodes, Integer input_layer)
{
	Integer input_nodes_count = network->layers[input_layer]->node_count;
	Integer links_count = input_nodes_count * nodes;

	ff_network_init_layer(network, layer, nodes, links_count);

	Network* out_l = network->layers[layer];
	Network* in_l = network->layers[input_layer];

	for (int i = 0; i < input_nodes_count; i++)
	{
		NetworkNode* input_node = in_l->nodes + i;
		for (int j = 0; j < nodes; j++)
		{
			NetworkLink* link = out_l->links + i * nodes + j;
			link->input = input_node;
			link->output = out_l->nodes + j;
			link->weight = 0.f;
		}
	}
}
