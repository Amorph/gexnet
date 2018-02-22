#ifndef __GEXNET_PROCESSORS_H__
#define __GEXNET_PROCESSORS_H__

#include "network.h"

typedef void(*ProcessLink)(Network* network, NetworkLink* link, NetworkNodeData* nodes_data);
typedef void(*ProcessNodeData)(Network* network, NetworkNode* node, NetworkNodeData* node_data);
typedef void(*ProcessNetwork)(Network* network, NetworkData* data, ProcessLink* link_processors, ProcessNodeData* data_processors);


void cpu_process_links(Network* network, NetworkData* data, ProcessLink* link_processors);
void cpu_process_nodes(Network* network, NetworkData* data, ProcessNodeData* node_processors);

void cpu_link_processor(Network* network, NetworkLink* link, NetworkNodeData* node_data);
void cpu_node_bias_processor(Network* network, NetworkNode* node, NetworkNodeData* node_data);
void cpu_node_clamp_processor(Network* network, NetworkNode* node, NetworkNodeData* node_data);
void cpu_node_zero_processor(Network* network, NetworkNode* node, NetworkNodeData* node_data);


#endif
