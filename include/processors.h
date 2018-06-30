#ifndef __GEXNET_PROCESSORS_H__
#define __GEXNET_PROCESSORS_H__

#include "network.h"

#define GEXNET_FUNCTION_TANH (0)

size_t gexnet_compute_node_count(NetworkStream* links);
bool gexnet_compute_in_out_streams(NetworkStream* links, size_t node_count, NetworkStream** inputs, NetworkStream** outputs);
void gexnet_process_links_weight(NetworkStream* links, NetworkStream* weight, NetworkStream* output);
void gexnet_process_nodes(NetworkStream* input_nodes, NetworkStream* out_nodes, size_t function);
void gexnet_process_node_sum_weighted_links(NetworkStream* nodes, NetworkStream* links, NetworkStream* weights, NetworkStream* prev_nodes);
void gexnet_process_stream_add(Network* network, FourCC target, FourCC source);

#endif
