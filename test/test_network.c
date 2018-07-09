#include "network.h"
#include "processors.h"
#include "feedforward.h"
#include "graph.h"
#include "graph_dot.h"
#include "builder.h"

#include "gexnet.h"
#include "stream.h"

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


void test_new_interface()
{
	struct GNSystem* G = gexnet_native_init(NULL);
	struct GNStream* links = G->create_stream(G, GN_TYPE_LINK, 6, NULL);

	G->stream->destroy(links);
	G->destroy(G);
}

void main()
{
	struct GNSystem* G = gexnet_native_init(NULL);

	test_new_interface();
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
	struct GNStream* links = G->create_stream(G, GN_TYPE_LINK, num_links, raw_links); 
	size_t node_count = G->compute->node_count(links);

	struct GNStream* inputs, *outputs;
	G->compute->in_out(links, node_count, &inputs, &outputs);

	// create weights
	struct GNStream* weights_stream = G->create_stream(G, GN_TYPE_FLOAT, num_links, NULL);
	struct GNStreamLockData* weights_lock = G->stream->lock(weights_stream, 0, num_links, 0);
	Number* weigths_data = weights_lock->data;
	for (size_t i = 0; i < num_links; i++)
	{
		weigths_data[i] = rand() / (float)RAND_MAX * 2.f - 1.f;
	}
	G->stream->unlock(weights_lock);
	
	// create data layers
	struct GNStream* data_stream0 = G->create_stream(G, GN_TYPE_FLOAT, node_count, NULL);
	struct GNStream* data_stream1 = G->create_stream(G, GN_TYPE_FLOAT, node_count, NULL);
	G->stream->clear(data_stream0);
	G->stream->clear(data_stream1);

	float in_data[] = { 1.f, 1.f, 1.f };
	G->stream->set_stream_data_indexed(data_stream0, inputs, in_data);

	// calculate weigths
	G->stream->multiply_add_links(data_stream1, links, data_stream0, weights_stream);
	G->stream->process_stream(data_stream0, data_stream1, GN_FUNCTION_TANH);

	G->stream->clear(data_stream1);
	G->stream->multiply_add_links(data_stream1, links, data_stream0, weights_stream);
	G->stream->process_stream(data_stream0, data_stream1, GN_FUNCTION_TANH);

	G->stream->clear(data_stream1);
	G->stream->multiply_add_links(data_stream1, links, data_stream0, weights_stream);
	G->stream->process_stream(data_stream0, data_stream1, GN_FUNCTION_TANH);
	// process weights - activation func
	
	// backpropagation experiments?

	NetworkGraph* graph = network_graph_create(links);
	FILE* f = fopen("gexnet.gv", "wt");

	network_graph_generate_dot(graph, fprintf, f);
	fclose(f);

	network_graph_destroy(graph);

	G->destroy(G);
	//gexnet_compute_forward_propagation(links, 0, 0, 0, 0, 0, 0);
	//gexnet_compute_in_out_streams(links, node_count, &inputs, &outputs);
}