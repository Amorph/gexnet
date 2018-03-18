#include "graph.h"
#include "memory.h"

#include "processors.h"


typedef struct
{
	size_t ilink_last;
	size_t olink_last;
}_NodeLinksCursor;

NetworkGraph*	network_graph_create(NetworkStream* links)
{
	if (!links)
		return NULL;

	size_t node_count = gexnet_compute_node_count(links);

	if (!node_count)
		return NULL;

	NetworkStream *inputs, *outputs;
	if (!gexnet_compute_in_out_streams(links, node_count, &inputs, &outputs))
		return NULL;
	
	NetworkStreamLockData* links_stream = network_stream_lock(links, 0, 0);
	NetworkStreamLockData* inputs_stream = network_stream_lock(inputs, 0, 0);
	NetworkStreamLockData* outputs_stream = network_stream_lock(outputs, 0, 0);

	if (!links_stream || !inputs_stream || !outputs_stream)
	{
		if (links_stream)
			network_stream_unlock(links_stream);
		if (inputs_stream)
			network_stream_unlock(inputs_stream);
		if (outputs_stream)
			network_stream_unlock(outputs_stream);
		return false;
	}

	NetworkLink* links_data = (NetworkLink*)links->data;

	size_t big_chunk_size = sizeof(NetworkGraph);
	big_chunk_size += sizeof(NetworkGraphNode) * node_count; // ff_nodes
	big_chunk_size += sizeof(NetworkGraphNode*) * node_count; // ff_layer_slots
	big_chunk_size += sizeof(NetworkGraphLink) * links_stream->count; // ff_links
	big_chunk_size += sizeof(NetworkGraphLink*) * links_stream->count; // ff_ilinks
	big_chunk_size += sizeof(NetworkGraphLink*) * links_stream->count; // ff_olinks

	uint8_t* big_chunk = allocator_get()->allocate(big_chunk_size);
	uint8_t* big_chunk_cursor = big_chunk;

	NetworkGraph*		graph =				(NetworkGraph*)big_chunk_cursor; big_chunk_cursor += sizeof(NetworkGraph);
	NetworkGraphNode*	ff_nodes =			(NetworkGraphNode*)big_chunk_cursor; big_chunk_cursor += sizeof(NetworkGraphNode) * node_count;
	NetworkGraphNode**	ff_layers_slots =	(NetworkGraphNode**)big_chunk_cursor; big_chunk_cursor += sizeof(NetworkGraphNode*) * node_count;
	NetworkGraphLink*	ff_links =			(NetworkGraphLink*)big_chunk_cursor; big_chunk_cursor += sizeof(NetworkGraphLink) * links_stream->count;
	NetworkGraphLink**	ff_ilinks =			(NetworkGraphLink**)big_chunk_cursor; big_chunk_cursor += sizeof(NetworkGraphLink*) * links_stream->count;
	NetworkGraphLink**	ff_olinks =			(NetworkGraphLink**)big_chunk_cursor; big_chunk_cursor += sizeof(NetworkGraphLink*) * links_stream->count;

	_NodeLinksCursor*	ff_links_cursor =	allocator_get()->allocate(sizeof(_NodeLinksCursor) * node_count);

	for (size_t i = 0; i < node_count; i++)
	{
		NetworkGraphNode* node = ff_nodes + i;

		node->layer = NETWORK_GRAPH_ORPHANED;
		node->node_index = i;
		node->ilinks_count = 0;
		node->olinks_count = 0;

		ff_links_cursor[i].ilink_last = 0;
		ff_links_cursor[i].olink_last = 0;
	}

	for (size_t i = 0; i < links_stream->count; i++)
	{
		NetworkGraphLink* link = ff_links + i;
		link->link_index = i;
		link->inode = ff_nodes + links_data[i].input;
		link->onode = ff_nodes + links_data[i].output;
		link->inode->olinks_count++;
		link->onode->ilinks_count++;
	}

	NetworkGraphLink** node_ilinks_heap = ff_ilinks;
	NetworkGraphLink** node_olinks_heap = ff_olinks;

	for (size_t i = 0; i < node_count; i++)
	{
		NetworkGraphNode* node = ff_nodes + i;
		node->ilinks = node->ilinks_count ? node_ilinks_heap : 0;
		node->olinks = node->olinks_count ? node_olinks_heap : 0;
		node_ilinks_heap += node->ilinks_count;
		node_olinks_heap += node->olinks_count;
	}

	for (size_t i = 0; i < links_stream->count; i++)
	{
		NetworkGraphLink* link = ff_links + i;
		NetworkGraphNode* inode = link->inode;
		NetworkGraphNode* onode = link->onode;
		inode->olinks[ff_links_cursor[inode->node_index].olink_last++] = link;
		onode->ilinks[ff_links_cursor[onode->node_index].ilink_last++] = link;
	}

	allocator_get()->free(ff_links_cursor);
	ff_links_cursor = 0;

	NetworkGraphLayer* ff_layers = allocator_get()->allocate(sizeof(NetworkGraphLayer) * (node_count - inputs_stream->count - outputs_stream->count + 2 + 1));

	NetworkGraphLayer* prev_layer, *input_layer, *output_layer;
	NetworkGraphNode** last_slot = ff_layers_slots;

	// Create output layer
	output_layer = ff_layers + 0;
	output_layer->index = 0;
	output_layer->nodes = last_slot;
	output_layer->nodes_count = outputs_stream->count;
	last_slot += output_layer->nodes_count;

	// Create input layer
	input_layer = ff_layers + 1;
	input_layer->index = 1;
	input_layer->nodes = last_slot;
	input_layer->nodes_count = inputs_stream->count;
	last_slot += input_layer->nodes_count;

	// Fill output layer
	size_t* outputs_data = outputs_stream->data;
	for (size_t i = 0; i < output_layer->nodes_count; i++)
	{
		NetworkGraphNode* node = ff_nodes + outputs_data[i];

		node->layer = output_layer->index;
		output_layer->nodes[i] = node;
	}
	// Fill input layer
	size_t* inputs_data = inputs_stream->data;
	for (size_t i = 0; i < input_layer->nodes_count; i++)
	{
		NetworkGraphNode* node = ff_nodes + inputs_data[i];

		node->layer = input_layer->index;
		input_layer->nodes[i] = node;
	}
	prev_layer = input_layer;
	while (true)
	{
		NetworkGraphLayer* current_layer = prev_layer + 1;
		current_layer->index = prev_layer->index + 1;
		current_layer->nodes = last_slot;
		current_layer->nodes_count = 0;
		for (size_t i = 0; i < prev_layer->nodes_count; i++)
		{
			NetworkGraphNode* node = prev_layer->nodes[i];
			for (size_t j = 0; j < node->olinks_count; j++)
			{
				NetworkGraphNode* candidate = node->olinks[j]->onode;
				size_t l;
				if (candidate->layer != NETWORK_GRAPH_ORPHANED)
					continue; // Already added
				for (l = 0; l < candidate->ilinks_count; l++)
					if (candidate->ilinks[l]->inode->layer == NETWORK_GRAPH_ORPHANED)
						break;
				if (l == candidate->ilinks_count)
				{
					candidate->layer = current_layer->index;
					current_layer->nodes[current_layer->nodes_count++] = candidate;
				}
			}
		}
		if (!current_layer->nodes_count)
			break;
		last_slot += current_layer->nodes_count;
		prev_layer = current_layer;
	}

	size_t layers_count = prev_layer->index + 1;

	NetworkGraphLayer* layers_final = allocator_get()->allocate(sizeof(NetworkGraphLayer) * layers_count);
	for (size_t i = 0; i < layers_count; i++)
	{
		NetworkGraphLayer* t_layer = layers_final + i;
		NetworkGraphLayer* s_layer = ff_layers + (i+1);
		// remap inputs-hidden-output
		if(i == layers_count - 1)
			s_layer = ff_layers + 0;

		t_layer->index = s_layer->index;
		t_layer->nodes = s_layer->nodes;
		t_layer->nodes_count = s_layer->nodes_count;
	}

	allocator_get()->free(ff_layers);

	graph->nodes = ff_nodes;
	graph->layers = layers_final;
	graph->layers_count = layers_count;

	allocator_get()->free(ff_links_cursor);
	return graph;
}

void network_graph_destroy(NetworkGraph* graph)
{
	allocator_get()->free(graph->layers);
	allocator_get()->free(graph);
}
