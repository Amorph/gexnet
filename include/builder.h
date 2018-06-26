#ifndef __GEXNET_BUILDER_H__
#define __GEXNET_BUILDER_H__

#include "types.h"
#include "network.h"


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
// struct NetworkBuilder* bld = network_create_builder() 
// size_t weights = bld->primitives->create_link_data(bld, TYPE_NUMBER, uniform_generator);
// size_t bias = bld->primitives->create_node_data(bld, TYPE_NUMBER, uniform_generator);
// size_t input_node_value = bld->primitives->input_node.value;
// size_t link_value = bld->primitives->input_link.value;
// builder_link_processor* link_processor = builder->create_link_processor(builder, bld->op->sum(bld->op->mul(input_node_value, weights)));
// builder_node_processor* node_processor = builder->create_node_processor(builder, bld->func->tanh(bld->op->add(link_value, bias));
// 
// Link generation TODO

typedef void(*LayerInitDataGet)(struct LayerInitDataProvider* obj, XYZIntVector index);
typedef void(*LayerInitDataProvider)(void* obj, XYZIntVector index);

typedef	size_t BuilderLinkAggregator;
typedef	size_t BuilderOperand;
typedef size_t BuilderNodeValueCompute;

struct LayerInitDataProvider
{
	LayerInitDataGet getter;
};


struct BuilderLayer;
struct BuilderLinkData;

enum BuilderDataType
{
	NUMBER,
	INTEGER
};

struct NetworkBuilder
{
	void (*destroy)(struct NetworkBuilder* builder);
	struct _BuilderNetworkIntefrace
	{
		struct BuilderLayer*	(*create_layer)			(struct NetworkBuilder* builder, XYZIntVector size, BuilderNodeValueCompute compute);
		struct BuilderLayer*	(*create_input_layer)	(struct NetworkBuilder* builder, XYZIntVector size);
		void					(*full_connection)		(struct NetworkBuilder* builder, struct BuilderLayer* input, struct BuilderLayer* layer);
		BuilderOperand			(*create_link_data)		(struct NetworkBuilder* builder, const char* name, enum BuilderDataType data_type, LayerInitDataProvider* data_provider);
		BuilderOperand			(*create_node_data)		(struct NetworkBuilder* builder, const char* name, enum BuilderDataType data_type, LayerInitDataProvider* data_provider);
		BuilderOperand			(*create_constant)		(struct NetworkBuilder* builder, const char* name, Number value);
		Network*				(*compile)				(struct NetworkBuilder* builder);
		BuilderNodeValueCompute	(*compile_node)			(struct NetworkBuilder* builder, BuilderLinkAggregator link_function, BuilderOperand node_function);
	}*network;

	struct _BuilderValuesInterface
	{
		struct BuilderLayer* last_layer;
		struct
		{
			BuilderOperand value;
		}input_node;
		struct
		{
			BuilderOperand value;
		}input_link;
	}*values;

	struct _BuilderLinkAggregationsIntefrace
	{
		BuilderLinkAggregator(*sum)(struct NetworkBuilder* builder, BuilderOperand input);
		BuilderLinkAggregator(*min)(struct NetworkBuilder* builder, BuilderOperand input);
		BuilderLinkAggregator(*avg)(struct NetworkBuilder* builder, BuilderOperand input);
		BuilderLinkAggregator(*max)(struct NetworkBuilder* builder, BuilderOperand input);
	}*links_aggregation;

	struct _BuilderOperationsInterface
	{
		BuilderOperand(*add)(struct NetworkBuilder* builder, BuilderOperand a, BuilderOperand b);
		BuilderOperand(*mul)(struct NetworkBuilder* builder, BuilderOperand a, BuilderOperand b);
		BuilderOperand(*sub)(struct NetworkBuilder* builder, BuilderOperand a, BuilderOperand b);
	}*op;

	struct _BuilderFunctionsInterface
	{
		BuilderOperand(*tahn)	(struct NetworkBuilder* builder, BuilderOperand x);
		BuilderOperand(*relu)	(struct NetworkBuilder* builder, BuilderOperand x);
		BuilderOperand(*sigmoid)(struct NetworkBuilder* builder, BuilderOperand x);
		BuilderOperand(*sign)	(struct NetworkBuilder* builder, BuilderOperand x);
	}*func;
};

struct NetworkBuilder*	network_create_builder();

#endif
