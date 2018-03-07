#include "network.h"
#include "processors.h"

void main()
{
	size_t neurons_connections = 2;

	NetworkStream* links = network_stream_create(FOURCC_LINK, sizeof(NetworkLink), neurons_connections);
	NetworkStream* inputs;
	NetworkStream* outputs;

	NetworkStreamLockData* neuron_links = network_stream_lock(links, 0, neurons_connections);
	NetworkLink* link_data = neuron_links->data;
	link_data[0].input = 0;
	link_data[0].output = 2;
	link_data[1].input = 1;
	link_data[1].output = 2;
	network_stream_unlock(neuron_links);

	size_t node_count = gexnet_compute_node_count(links);

	gexnet_compute_in_out_streams(links, node_count, &inputs, &outputs);
}