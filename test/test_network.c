#include "network.h"
#include "processors.h"
#include "feedforward.h"
#include "graph.h"
#include "graph_dot.h"
#include "builder.h"

#include <stdio.h>
#include <stdlib.h>

XYZIntVector net_input_dim = { 3, 1, 1 };
XYZIntVector net_output_dim = { 2, 1, 1 };

void test_builder()
{
	struct NetworkBuilder* bld = network_create_builder();

	BuilderOperand weights = bld->network->create_link_data(bld, "weights", NUMBER, NULL);
	BuilderOperand bias = bld->network->create_node_data(bld, "bias", NUMBER, NULL);

	BuilderLinkAggregator link_compute = bld->links_aggregation->sum(bld, bld->op->mul(bld, bld->values->input_node.value, weights));
	BuilderLinkAggregator node_value_compute = bld->func->sigmoid(bld, bld->op->add(bld, bld->values->input_link.value, bias));
	BuilderNodeValueCompute node_compute = bld->network->compile_node(bld, link_compute, node_value_compute);

	struct BuilderLayer* input_layer = bld->network->create_input_layer(bld, net_input_dim);
	struct BuilderLayer* output_layer = bld->network->create_layer(bld, net_output_dim, node_compute);

	bld->network->full_connection(bld, input_layer, output_layer);

	Network* net = bld->network->compile(bld);

	bld->destroy(bld);
}

NetworkStream* create_links_stream(NetworkLink links[], size_t count)
{
	NetworkStream* stream = network_stream_create(FOURCC_LINK, sizeof(NetworkLink), count);

	NetworkStreamLockData* neuron_links = network_stream_lock(stream, 0, count);
	NetworkLink* link_data = neuron_links->data;
	for (size_t i = 0; i < count; i++)
		link_data[i] = links[i];

	network_stream_unlock(neuron_links);
	return stream;
}

void main()
{
	size_t neurons_connections = 6;

	//0   6
	//1 4 7 10
	//2 5 8 11
	//3   9

	//0   4
	//1 3 
	//2   5
	//test_builder();
	NetworkLink raw_links[] = {
		{ 0, 3 },
		{ 1, 3 },
		{ 2, 3 },
		{ 3, 3 },
		{ 3, 4 },
		{ 3, 5 },
		{ 5, 6 },
		{ 4, 6 },
		{ 3, 6 }
	};
	size_t num_links = sizeof(raw_links) / sizeof(NetworkLink);
	NetworkStream* links = create_links_stream(raw_links, num_links);
	size_t node_count = gexnet_compute_node_count(links);

	NetworkStream* inputs, *outputs;
	gexnet_compute_in_out_streams(links, node_count, &inputs, &outputs);

	// create weights
	NetworkStream* weights_stream = network_stream_create(FOURCC_LINK_WEIGHT, sizeof(Number), num_links);
	NetworkStreamLockData* weights_lock = network_stream_lock(weights_stream, 0, num_links);
	Number* weigths_data = weights_lock->data;
	for (size_t i = 0; i < num_links; i++)
	{
		weigths_data[i] = rand() / (float)RAND_MAX * 2.f - 1.f;
	}
	network_stream_unlock(weights_lock);

	// create data layers
	NetworkStream* data_stream0 = network_stream_create(FOURCC_DATA, sizeof(Number), node_count);
	NetworkStream* data_stream1 = network_stream_create(FOURCC_DATA, sizeof(Number), node_count);
	network_stream_clear(data_stream0);
	network_stream_clear(data_stream1);
	float in_data[] = { 1.f, 1.f, 1.f };
	network_stream_set_indexed(data_stream0, inputs, in_data, 0, 3);

	// calculate weigths
	network_stream_clear(data_stream1);
	gexnet_process_node_sum_weighted_links(data_stream1, links, weights_stream, data_stream0);
	gexnet_process_nodes(data_stream1, data_stream1, GEXNET_FUNCTION_TANH);
	network_stream_clear(data_stream0);
	gexnet_process_node_sum_weighted_links(data_stream0, links, weights_stream, data_stream1);
	gexnet_process_nodes(data_stream0, data_stream0, GEXNET_FUNCTION_TANH);
	// process weights - activation func
	
	// backpropagation experiments?

	NetworkGraph* graph = network_graph_create(links);
	FILE* f = fopen("gexnet.gv", "wt");

	network_graph_generate_dot(graph, fprintf, f);
	fclose(f);

	network_graph_destroy(graph);

	//gexnet_compute_forward_propagation(links, 0, 0, 0, 0, 0, 0);
	//gexnet_compute_in_out_streams(links, node_count, &inputs, &outputs);
}