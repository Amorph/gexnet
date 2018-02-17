#include "network.h"
#include "memory.h"

network* create_network(integer node_count, integer links_count)
{
	network* net = get_allocator()->allocate(sizeof(network));
	net->node_count = node_count;
	net->links_count = links_count;
	net->nodes = get_allocator()->allocate(sizeof(*net->nodes) * node_count);
	net->links = get_allocator()->allocate(sizeof(*net->links) * links_count);
	return net;
}

network_data* create_network_data(integer node_count)
{
	network_data* data = get_allocator()->allocate(sizeof(network_data));
	data->node_count = node_count;
	data->node_data = get_allocator()->allocate(sizeof(*data->node_data) * node_count);
	return data;
}

void destroy_network(network* net)
{
	get_allocator()->free(net->nodes);
	get_allocator()->free(net->links);
	get_allocator()->free(net);
}

void destroy_network_data(network_data* data)
{
	get_allocator()->free(data->node_data);
	get_allocator()->free(data);
}
