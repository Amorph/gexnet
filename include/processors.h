#ifndef __GEXNET_PROCESSORS_H__
#define __GEXNET_PROCESSORS_H__

#include "network.h"

void gexnet_compute_in_out_streams(Network* network);
void gexnet_process_links(Network* network);
void gexnet_process_links_weight(Network* network, FourCC weight_data);
void gexnet_process_stream_add(Network* network, FourCC target, FourCC source);

#endif
