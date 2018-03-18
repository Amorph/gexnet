#ifndef __GEXNET_GRAPH_DOT_H__
#define __GEXNET_GRAPH_DOT_H__

#include "graph.h"

typedef void(*NetworkGraphPrinter)(void* obj, const char* fmt, ...);

void network_graph_generate_dot(NetworkGraph* graph, NetworkGraphPrinter printer, void* obj);


#endif