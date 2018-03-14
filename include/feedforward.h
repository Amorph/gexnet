#ifndef __GEXNET_FEEDFORWARD_NETWORK_H__
#define __GEXNET_FEEDFORWARD_NETWORK_H__

#include "types.h"
#include "network.h"

bool gexnet_compute_forward_propagation(NetworkStream* links, size_t node_count, NetworkStream* inputs, NetworkStream* outputs, NetworkStream** fflayers, NetworkStream** ffnodes, NetworkStream** fflinks);

#endif
