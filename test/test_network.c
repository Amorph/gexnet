#include "network.h"
#include "processors.h"

void main()
{
	size_t neurons_count = 3;
	size_t neurons_connections = 2;
	Network* network = network_create();

	NetworkStream* neurons = network_stream_create(FOURCC_DATA, 0, neurons_count);
	NetworkStream* links = network_stream_create(FOURCC_LINK, sizeof(NetworkLink), neurons_connections);

	NetworkStreamLockData* neuron_links = network_stream_lock(links, 0, neurons_connections);
	NetworkLink* link_data = neuron_links->data;
	link_data[0].input = 0;
	link_data[0].output = 2;
	link_data[1].input = 1;
	link_data[1].output = 2;
	network_stream_unlock(neuron_links);

	network_attach_stream(network, neurons);
	network_attach_stream(network, links);
	gexnet_compute_in_out_streams(network);

	network_destroy(network);
}