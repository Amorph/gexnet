#include "network.h"
#include "processors.h"
#include "feedforward.h"
#include "graph.h"
#include "graph_dot.h"
#include "builder.h"

#include <stdio.h>

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

void spinking_nn()
{
	// links
	// links weights
	// node data
	// prev frame node data
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

	NetworkStream* links = network_stream_create(FOURCC_LINK, sizeof(NetworkLink), neurons_connections);
	NetworkStream* inputs;
	NetworkStream* outputs;

	NetworkStreamLockData* neuron_links = network_stream_lock(links, 0, neurons_connections);
	NetworkLink* link_data = neuron_links->data;
	link_data[0].input = 0;	link_data[0].output = 3;
	link_data[1].input = 1;	link_data[1].output = 3;
	link_data[2].input = 2;	link_data[2].output = 3;
	link_data[3].input = 3;	link_data[3].output = 4;
	link_data[4].input = 3;	link_data[4].output = 3;
	link_data[5].input = 3;	link_data[5].output = 5;

	network_stream_unlock(neuron_links);

	size_t node_count = gexnet_compute_node_count(links);
	NetworkGraph* graph = network_graph_create(links);
	FILE* f = fopen("gexnet.gv", "wt");

	network_graph_generate_dot(graph, fprintf, f);
	fclose(f);

	network_graph_destroy(graph);

	//gexnet_compute_forward_propagation(links, 0, 0, 0, 0, 0, 0);
	//gexnet_compute_in_out_streams(links, node_count, &inputs, &outputs);
}