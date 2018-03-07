#ifndef __GEXNET_PROCESSORS_H__
#define __GEXNET_PROCESSORS_H__

#include "network.h"

size_t gexnet_compute_node_count(NetworkStream* links);
void gexnet_compute_in_out_streams(NetworkStream* links, size_t node_count, NetworkStream** inputs, NetworkStream** outputs);
void gexnet_process_links_weight(NetworkStream* links, NetworkStream* weight, NetworkStream* output);
void gexnet_process_stream_add(Network* network, FourCC target, FourCC source);

#endif
