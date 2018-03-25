#include "builder.h"
#include "memory.h"

struct InputLayer
{
	struct BaseLayer base;
	size_t* inputs;
	size_t*	outputs;
};

struct DuplicateLayer
{
	struct BaseLayer	base;
	XYZIntVector		mult;
};

struct ConvolutionLayer
{
	struct BaseLayer		base;
	XYZIntVector			kernel, padding, stride;
	LayerInitDataProvider*	filter_weights;
};

struct BaseLayer* layer_linear_input(XYZIntVector dim)
{
	struct InputLayer* layer = allocator_get()->allocate(sizeof(struct InputLayer));

	layer->base.in = dim;
	layer->base.out = dim;
	layer->base.type = LAYER_INPUT;
	layer->base.inputs = 0;
	layer->base.outputs = 0;

	return (struct BaseLayer*)layer;
}

BaseLayer* layer_duplicate(BaseLayer* input, XYZIntVector mult)
{
	struct DuplicateLayer* layer = allocator_get()->allocate(sizeof(struct DuplicateLayer));
	XYZIntVector out = { input->in.x * mult.x, input->in.y * mult.y, input->in.z * mult.z };

	layer->base.in = input->in;
	layer->base.out = out;
	layer->base.type = LAYER_DUP;
	layer->base.inputs = 0;
	layer->base.outputs = 0;
	layer->mult = mult;

	return (struct BaseLayer*)layer;
}

BaseLayer* layer_convolution(BaseLayer* input, XYZIntVector kernel, XYZIntVector padding, XYZIntVector stride, LayerInitDataProvider* filter_weights)
{

}