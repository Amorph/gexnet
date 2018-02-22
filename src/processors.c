#include "processors.h"

void cpu_process_links(Network* network, NetworkData* data, ProcessLink* link_processors)
{
	ProcessLink* processor = link_processors;
	while (*processor)
	{
		for (size_t i = 0; i < network->links_count; i++)
			(*processor)(network, network->links + i, data->node_data);
		processor++;
	}
}

void cpu_process_nodes(Network* network, NetworkData* data, ProcessNodeData* node_processors)
{
	ProcessNodeData* processor = node_processors;
	while (*processor)
	{
		for (size_t i = 0; i < network->node_count; i++)
			(*processor)(network, network->nodes + i, data->node_data);
		processor++;
	}
}

void cpu_link_processor(Network* network, NetworkLink* link, NetworkNodeData* node_data)
{
	NetworkNodeData* input = node_data + link->input;
	NetworkNodeData* output = node_data + link->output;
	output->value += input->value * link->weight;
}

void cpu_node_bias_processor(Network* network, NetworkNode* node, NetworkNodeData* node_data)
{

}

void cpu_node_clamp_processor(Network* network, NetworkNode* node, NetworkNodeData* node_data)
{

}

void cpu_node_zero_processor(Network* network, NetworkNode* node, NetworkNodeData* node_data)
{

}
