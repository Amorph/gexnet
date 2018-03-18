#ifndef __GEXNET_GRAPH_H__
#define __GEXNET_GRAPH_H__

#include "types.h"
#include "network.h"

#define NETWORK_GRAPH_INPUT_LAYER (1)
#define NETWORK_GRAPH_OUTPUT_LAYER (0)
#define NETWORK_GRAPH_ORPHANED (-1)

typedef struct _NetworkGraphLayer NetworkGraphLayer;
typedef struct _NetworkGraphLink NetworkGraphLink;

typedef struct
{
	size_t				node_index;
	size_t				layer;
	size_t				ilinks_count;
	size_t				olinks_count;
	NetworkGraphLink**	ilinks;
	NetworkGraphLink**	olinks;
}NetworkGraphNode;

typedef struct _NetworkGraphLink
{
	NetworkGraphNode*	inode;
	NetworkGraphNode*	onode;
	size_t				link_index;
}NetworkGraphLink;

typedef struct _NetworkGraphLayer
{
	size_t				index;
	NetworkGraphNode**	nodes;
	size_t				nodes_count;
}NetworkGraphLayer;

typedef struct
{
	size_t				layers_count;
	NetworkGraphLayer*	layers;
	NetworkGraphNode*	nodes;
}NetworkGraph;


NetworkGraph*	network_graph_create(NetworkStream* links);
void			network_graph_destroy(NetworkGraph* graph);

#endif