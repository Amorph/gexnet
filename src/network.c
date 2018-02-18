#include "network.h"
#include "memory.h"

#include "string.h"

Network* network_create(Integer node_count, Integer links_count)
{
	Network* net = allocator_get()->allocate(sizeof(Network));
	net->node_count = node_count;
	net->links_count = links_count;
	net->nodes = allocator_get()->allocate(sizeof(*net->nodes) * node_count);
	net->links = allocator_get()->allocate(sizeof(*net->links) * links_count);

	memset(net->nodes, 0, sizeof(*net->nodes) * node_count);
	memset(net->links, 0, sizeof(*net->links) * links_count);

	return net;
}

NetworkData* network_create_data(Integer node_count)
{
	NetworkData* data = allocator_get()->allocate(sizeof(NetworkData));
	data->node_count = node_count;
	data->node_data = allocator_get()->allocate(sizeof(*data->node_data) * node_count);
	return data;
}

void network_destroy(Network* net)
{
	allocator_get()->free(net->nodes);
	allocator_get()->free(net->links);
	allocator_get()->free(net);
}

void network_destroy_data(NetworkData* data)
{
	allocator_get()->free(data->node_data);
	allocator_get()->free(data);
}
