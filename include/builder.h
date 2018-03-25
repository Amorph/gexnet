#ifndef __GEXNET_BUILDER_H__
#define __GEXNET_BUILDER_H__

#include "types.h"

#define LAYER_INPUT (0)
#define LAYER_DUP	(1)
#define LAYER_CONV	(2)
#define LAYER_MUL	(3)
#define LAYER_ADD	(4)

typedef struct BaseLayer
{
	size_t			type;
	XYZIntVector	in;
	XYZIntVector	out;
	size_t*			inputs;
	size_t*			outputs;
}BaseLayer;

struct NetworkBuilder
{
	BaseLayer**	layers;
	size_t		layers_count;
}NetworkBuilder;

typedef void(*LayerInitDataGet)(struct LayerInitDataProvider* obj, XYZIntVector index);
typedef void(*LayerInitDataProvider)(void* obj, XYZIntVector index);

struct LayerInitDataProvider
{
	LayerInitDataGet getter;
};

BaseLayer*	layer_linear_input(XYZIntVector dim);
BaseLayer*	layer_duplicate(BaseLayer* input, XYZIntVector mult);
BaseLayer*	layer_convolution(BaseLayer* input, XYZIntVector kernel, XYZIntVector padding, XYZIntVector stride, LayerInitDataProvider* filter_weights);
// Identical to:

/*BaseLayer*	layer_full_connection(BaseLayer* input, XYZIntVector size, LayerInitDataProvider* weights, LayerInitDataProvider* biases);
BaseLayer*	layer_join(BaseLayer* a, BaseLayer* b);
BaseLayer*	layer_mul(BaseLayer* input, LayerInitDataProvider* data);
BaseLayer*	layer_add(BaseLayer* input, LayerInitDataProvider* data);*/
// TODO activation layer ?

// input = layer_linear_input(10);
// hidden = layer_full_connection(input, 15, weights, biases);
// output = layer_full_connection(hidden, 3, weights, biases);

//convolution output size calculation:
// OutSize = (SourceSize + 2 * Padding - FilterSize) / Stride + 1 


// New conception:
// Builder has ability to create separate formula for link and node:
// When connect one layer to another we need to specify:
// 1. Link generation formula, full connection, duplicate links, convolution
// 2. Compute link weight formula
// 3. Node value compute formula
// 
// Each formula can use variables that comes with DataProvider. DataProvider can be setted by default and can be overrided by layer.
// Also formula can use constants with float type
// Possible usage example:
// network_builder* bld = network_create_builder() 
// size_t weights = bld->create_link_data(bld, TYPE_NUMBER, uniform_generator);
// size_t bias = bld->create_node_data(bld, TYPE_NUMBER, uniform_generator);
// size_t input_node_value = bld->input_node->value;
// size_t link_value = bld->input_link->value;
// builder_link_processor* link_processor = builder->create_link_processor(builder, bld->op->mul(input_node_value, weights));
// builder_node_processor* node_processor = builder->create_node_processor(builder, bld->func->tanh(bld->op->add(bld->op->sum(link_value), bias));
// 
// Link generation TODO
#endif