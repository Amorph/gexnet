#include "builder.h"
#include "memory.h"

#define INPUT_NODE_VALUE (0) // TODO
#define INPUT_LINK_VALUE (0) // TODO

struct NetworkBuilderImpl
{
	struct NetworkBuilder pub;
};

void init_public_interface(struct NetworkBuilderImpl* builder);

static void 
builder_destroy(struct NetworkBuilder* builder)
{
	allocator_get()->free(builder);
}

static struct BuilderLayer* 
primitives_create_input(struct NetworkBuilder* builder, XYZIntVector size)
{
	return NULL;
}

static BuilderOperand
primitives_create_link_data(struct NetworkBuilder* builder, enum BuilderDataType data_type, LayerInitDataProvider* data_provider)
{
	return -1;
}

static BuilderOperand
primitives_create_node_data(struct NetworkBuilder* builder, enum BuilderDataType data_type, LayerInitDataProvider* data_provider)
{
	return -1;
}

static BuilderOperand
primitives_create_constant(struct NetworkBuilder* builder, Number value)
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

static struct _BuilderPrimitivesIntefrace 
primitives_interface = {
	.create_input = primitives_create_input,
	.create_link_data = primitives_create_link_data,
	.create_node_data = primitives_create_node_data,
	.create_constant = primitives_create_constant,
	.input_node = {.value = INPUT_NODE_VALUE },
	.input_link = {.value = INPUT_LINK_VALUE }
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
	builder->pub.primitives = &primitives_interface;
	builder->pub.op = &operations_interface;
	builder->pub.func = &functions_interface;
	
	return (struct NetworkBuilder*)builder;
}
