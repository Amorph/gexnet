#include "graph_dot.h"

void network_graph_generate_dot(NetworkGraph* graph, NetworkGraphPrinter printer, void* o)
{
	printer(o, "digraph GexNet \n{\n");
	//Settings
	printer(o,
		"rankdir = LR;\n"
		"splines=line;\n"
		"nodesep = 0.1;\n"
		"node [label=\"\", style=solid, color=black, shape=circle];\n"
		"edge [arrowsize=0.25];\n");

	//generate output virtual nodes
	NetworkGraphLayer* outputs = graph->layers + graph->layers_count - 1;
	printer(o,
		"subgraph virtual_output {\n"
		"\tnode [shape=none];\n\t");
	for (size_t i = 0; i < outputs->nodes_count; i++)
	{
		printer(o, "N%zx_out ", outputs->nodes[i]->node_index);
	}
	printer(o, ";\n}\n");

	//generate input virtual nodes
	NetworkGraphLayer* inputs = graph->layers + 0;
	printer(o,
		"subgraph virtual_input {\n"
		"\tnode [shape=none];\n\t");
	for (size_t i = 0; i < inputs->nodes_count; i++)
	{
		printer(o, "N%zx_in ", inputs->nodes[i]->node_index);
	}
	printer(o, ";\n}\n");

	for (size_t i = 0; i < graph->layers_count; i++)
	{
		NetworkGraphLayer* layer = graph->layers + i;
		printer(o, "subgraph cluster_%zu {\n", i);
		if (i == 0)
			printer(o, "\tlabel=\"Input\";\n");
		else if (i == graph->layers_count - 1)
			printer(o, "\tlabel=\"Output\";\n");
		else
			printer(o, "\tlabel=\"Layer %zu\";\n", i);
		printer(o, "\tcolor=gray;\n\t");
		for (size_t j = 0; j < layer->nodes_count; j++)
		{
			printer(o, "N%zx[label=\"%zu\"] ", layer->nodes[j]->node_index, layer->nodes[j]->node_index);
		}
		printer(o, ";\n}\n");
	}
	for (size_t i = 0; i < graph->layers_count; i++)
	{
		NetworkGraphLayer* layer = graph->layers + i;
		for (size_t j = 0; j < layer->nodes_count; j++)
		{
			NetworkGraphNode* node = layer->nodes[j];
			for (size_t l = 0; l < node->olinks_count; l++)
				printer(o, "N%zx -> N%zx;\n", node->node_index, node->olinks[l]->onode->node_index);
		}
	}

	for (size_t i = 0; i < outputs->nodes_count; i++)
	{
		printer(o, "N%zx -> N%zx_out;\n", outputs->nodes[i]->node_index, outputs->nodes[i]->node_index);
	}
	for (size_t i = 0; i < inputs->nodes_count; i++)
	{
		printer(o, "N%zx_in -> N%zx;\n", inputs->nodes[i]->node_index, inputs->nodes[i]->node_index);
	}

	printer(o, "}");
}