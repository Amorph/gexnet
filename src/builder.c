#include "builder.h"
#include "memory.h"

#define INPUT_NODE_VALUE (0) // TODO
#define INPUT_LINK_VALUE (0) // TODO

enum _PrimitiveType
{
	PT_LAYER,
	PT_NODE_COMPLETE_COMPUTE,
	PT_NODE_LINKS_AGGREGATOR,
	PT_NODE_FUNCTION,
	PT_DATA_CONSTANT,
	PT_DATA_LINK,
	PT_DATA_NODE,
	PT_LINK_ARRAY,
};

enum _LayerType
{
	LT_INPUT_LAYER = 0,
	LT_COMPUTE_LAYER = 1,
};

struct NetworkBuilderImpl
{
	struct NetworkBuilder pub;
};

struct BuilderLayer
{
	enum _LayerType			type;
	XYZIntVector			size;
	BuilderNodeValueCompute	compute;
};

static void 
builder_destroy(struct NetworkBuilder* builder)
{
	allocator_get()->free(builder);
}

static struct BuilderLayer* 
network_create_layer(struct NetworkBuilder* builder, XYZIntVector size, BuilderNodeValueCompute compute)
{
	struct BuilderLayer* layer = allocator_get()->allocate(sizeof(struct BuilderLayer));

	layer->type = LT_COMPUTE_LAYER;
	layer->size = size;
	layer->compute = compute;
	return layer;
}

static struct BuilderLayer*
network_create_input_layer(struct NetworkBuilder* builder, XYZIntVector size)
{
	struct BuilderLayer* layer = allocator_get()->allocate(sizeof(struct BuilderLayer));

	layer->type = LT_INPUT_LAYER;
	layer->size = size;
	layer->compute = 0;

	return layer;
}

static BuilderOperand
network_create_link_data(struct NetworkBuilder* builder, const char* name, enum BuilderDataType data_type, LayerInitDataProvider* data_provider)
{
	return -1;
}

static BuilderOperand
network_create_node_data(struct NetworkBuilder* builder, const char* name, enum BuilderDataType data_type, LayerInitDataProvider* data_provider)
{
	return -1;
}

static BuilderOperand
network_create_constant(struct NetworkBuilder* builder, const char* name, Number value)
{
	return -1;
}

static Network* 
network_compile(struct NetworkBuilder* builder)
{
	return NULL;
}
static BuilderNodeValueCompute
network_compile_node(struct NetworkBuilder* builder, BuilderLinkAggregator link_function, BuilderOperand node_function)
{
	return -1;
}

BuilderLinkAggregator
links_aggregation_sum(struct NetworkBuilder* builder, BuilderOperand input)
{
	return -1;
}

BuilderLinkAggregator
links_aggregation_min(struct NetworkBuilder* builder, BuilderOperand input)
{
	return -1;
}

BuilderLinkAggregator
links_aggregation_avg(struct NetworkBuilder* builder, BuilderOperand input)
{
	return -1;
}

BuilderLinkAggregator
links_aggregation_max(struct NetworkBuilder* builder, BuilderOperand input)
{
	return -1;
}

BuilderOperand
op_add(struct NetworkBuilder* builder, BuilderOperand a, BuilderOperand b)
{
	return -1;
}

BuilderOperand
op_mul(struct NetworkBuilder* builder, BuilderOperand a, BuilderOperand b)
{
	return -1;
}

BuilderOperand
op_sub(struct NetworkBuilder* builder, BuilderOperand a, BuilderOperand b)
{
	return -1;
}

BuilderOperand
func_tahn(struct NetworkBuilder* builder, BuilderOperand x)
{
	return -1;
}

BuilderOperand
func_relu(struct NetworkBuilder* builder, BuilderOperand x)
{
	return -1;
}

BuilderOperand
func_sigmoid(struct NetworkBuilder* builder, BuilderOperand x)
{
	return -1;
}

BuilderOperand
func_sign(struct NetworkBuilder* builder, BuilderOperand x)
{
	return -1;
}

static struct _BuilderLinkAggregationsIntefrace 
aggregations_interface = {
	.sum = links_aggregation_sum,
	.min = links_aggregation_min,
	.avg = links_aggregation_avg,
	.max = links_aggregation_max};

static struct _BuilderNetworkIntefrace
network_interface = {
	.create_layer = network_create_layer,
	.create_input_layer = network_create_input_layer,
	.create_link_data = network_create_link_data,
	.create_node_data = network_create_node_data,
	.create_constant = network_create_constant,
	.compile = network_compile,
	.compile_node = network_compile_node
};

static struct _BuilderValuesInterface
values_interface = {
	.last_layer = NULL,
	.input_node = { .value = INPUT_NODE_VALUE },
	.input_link = { .value = INPUT_LINK_VALUE }
};

static struct _BuilderOperationsInterface
operations_interface = {
	.add = op_add,
	.mul = op_mul,
	.sub = op_sub
};

static struct _BuilderFunctionsInterface
functions_interface = {
	.tahn = func_tahn,
	.relu = func_relu,
	.sigmoid = func_sigmoid,
	.sign = func_sign
};

struct NetworkBuilder*
	network_create_builder()
{
	struct NetworkBuilderImpl* builder = allocator_get()->allocate(sizeof(struct NetworkBuilderImpl));

	builder->pub.destroy = builder_destroy;

	builder->pub.links_aggregation = &aggregations_interface;
	builder->pub.network = &network_interface;
	builder->pub.values = &values_interface;
	builder->pub.op = &operations_interface;
	builder->pub.func = &functions_interface;
	
	return (struct NetworkBuilder*)builder;
}
