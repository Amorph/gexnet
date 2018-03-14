#include "feedforward.h"
#include "memory.h"
#include "processors.h"

#include <string.h>

typedef struct _FFLayer FFLayer;
typedef struct _FFLink FFLink;

typedef struct
{
	size_t		node_index;
	FFLayer*	layer;
	size_t		ilinks_count;
	size_t		olinks_count;
	size_t		ilink_last;
	size_t		olink_last;
	FFLink**	ilinks;
	FFLink**	olinks;
}FFNode;

typedef struct _FFLink
{
	FFNode*		inode;
	FFNode*		onode;
	size_t		link_index;
}FFLink;

typedef struct _FFLayer
{
	size_t		index;
	FFNode**	nodes;
	size_t		nodes_count;
}FFLayer;

bool gexnet_compute_forward_propagation(NetworkStream* links, size_t node_count, NetworkStream* inputs, NetworkStream* outputs, NetworkStream** fflayers, NetworkStream** ffnodes, NetworkStream** fflinks)
{
	if (!links)
		return false;

	if (!node_count)
		node_count = gexnet_compute_node_count(links);

	if (!node_count)
		return false;
	if (!inputs || !outputs)
	{
		NetworkStream** in = inputs ? 0 : &inputs;
		NetworkStream** out = outputs ? 0 : &outputs;
		gexnet_compute_in_out_streams(links, node_count, in, out);
	}

	if (!inputs || !outputs)
		return false;

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

	FFNode*	ff_nodes = allocator_get()->allocate(sizeof(FFNode) * node_count);
	FFLink* ff_links = allocator_get()->allocate(sizeof(FFLink) * links->elementCount);
	FFLink** ff_ilinks = allocator_get()->allocate(sizeof(FFLink*) * links->elementCount);
	FFLink** ff_olinks = allocator_get()->allocate(sizeof(FFLink*) * links->elementCount);

	for (size_t i = 0; i < node_count; i++)
	{
		FFNode* node = ff_nodes + i;
		
		node->node_index = i;

		node->ilinks_count = 0;
		node->olinks_count = 0;
		node->ilink_last = 0;
		node->olink_last = 0;
		node->layer = 0;
	}

	for (size_t i = 0; i < links_stream->count; i++)
	{
		FFLink* link = ff_links + i;
		link->link_index = i;
		link->inode = ff_nodes + links_data[i].input;
		link->onode = ff_nodes + links_data[i].output;
		link->inode->olinks_count++;
		link->onode->ilinks_count++;
	}

	FFLink** node_ilinks_heap = ff_ilinks;
	FFLink** node_olinks_heap = ff_olinks;

	for (size_t i = 0; i < node_count; i++)
	{
		FFNode* node = ff_nodes + i;
		node->ilinks = node->ilinks_count ? node_ilinks_heap : 0;
		node->olinks = node->olinks_count ? node_olinks_heap : 0;
		node_ilinks_heap += node->ilinks_count;
		node_olinks_heap += node->olinks_count;
	}

	for (size_t i = 0; i < links_stream->count; i++)
	{
		FFLink* link = ff_links + i;
		FFNode* inode = link->inode;
		FFNode* onode = link->onode;
		inode->olinks[inode->olink_last++] = link;
		onode->ilinks[onode->ilink_last++] = link;
	}

	FFLayer* ff_layers = allocator_get()->allocate(sizeof(FFLayer*) * (node_count - inputs_stream->count - outputs_stream->count + 2));
	FFNode** ff_layers_slots = allocator_get()->allocate(sizeof(FFNode*) * (node_count));

	// init input layer
	FFLayer* prev_layer = ff_layers;
	FFNode** last_slot = ff_layers_slots;

	prev_layer->index = 0;
	prev_layer->nodes = last_slot;
	prev_layer->nodes_count = inputs_stream->count;
	last_slot += prev_layer->nodes_count;

	size_t* inputs_data = inputs_stream->data;
	for (size_t i = 0; i < prev_layer->nodes_count; i++)
	{
		FFNode* node = ff_nodes + inputs_data[i];

		node->layer = prev_layer;
		prev_layer->nodes[i] = node;
	}

	return true;
}