#ifndef __GEXNET_BUILDER_H__
#define __GEXNET_BUILDER_H__

#include "types.h"

typedef struct
{
	size_t		layer_type;
}BaseLayer;

typedef struct
{
	BaseLayer**	layers;
	size_t		layers_count;
}NetworkBuilder;

typedef void(*LayerInitDataGet)(struct LayerInitDataProvider* obj, size_t index);
typedef void(*LayerInitDataProvider)(void* obj, size_t index);

struct LayerInitDataProvider
{
	LayerInitDataGet getter;
};

BaseLayer*	layer_linear_input(size_t count);
// Same as
// layer = layer_add(layer_sum(layer_mul(layer_duplicate(input, count), weights), count), biases);
BaseLayer*	layer_full_connection(BaseLayer* input, size_t count, LayerInitDataProvider* weights, LayerInitDataProvider* biases);
BaseLayer*	layer_duplicate(BaseLayer* input, size_t count);
BaseLayer*	layer_sum(BaseLayer* input, size_t output);
BaseLayer*	layer_mul(BaseLayer* input, LayerInitDataProvider* data);
BaseLayer*	layer_add(BaseLayer* input, LayerInitDataProvider* data);
// TODO activation layer ?

// input = layer_linear_input(10);
// hidden = layer_full_connection(input, 15, weights, biases);
// output = layer_full_connection(hidden, 3, weights, biases);

#endif