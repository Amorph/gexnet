#ifndef __GEXNET_FEEDFORWARD_NETWORK_H__
#define __GEXNET_FEEDFORWARD_NETWORK_H__

#include "types.h"
#include "network.h"

typedef struct
{
	Integer		layers_count;
	Network**	layers;
}FFNetwork;

FFNetwork*		ff_network_create(Integer layers_count);
void			ff_network_destroy(FFNetwork* network);

void			ff_network_init_layer(FFNetwork* network, Integer layer, Integer nodes, Integer links);
void			ff_network_init_fully_connected_layer(FFNetwork* network, Integer layer, Integer nodes, Integer input_layer);


#endif
