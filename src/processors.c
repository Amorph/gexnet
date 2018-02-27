#include "processors.h"
#include "memory.h"

#include <string.h>


typedef struct
{
	size_t in;
	size_t out;
} in_out_counter;

void gexnet_process_in_out_streams(Network* network)
{
	NetworkStream* nodes = network_get_stream_type(network, FOURCC_DATA);
	NetworkStream* links = network_get_stream_type(network, FOURCC_LINK);

	if (!nodes || !links)
		return;

	NetworkLink* links_data = (NetworkLink*)links->data;
	size_t nodes_count = nodes->elementCount;

	in_out_counter* counter = allocator_get()->allocate(sizeof(in_out_counter) * nodes_count);
	memset(counter, 0, sizeof(in_out_counter) * nodes_count);

	for (size_t i = 0; i < links->elementCount; i++)
	{
		counter[links_data[i].input].out++;
		counter[links_data[i].output].in++;
	}

	size_t inputs_count = 0, outputs_count = 0;
	for (size_t i = 0; i < nodes_count; i++)
	{
		if (counter[i].out && !counter[i].in)
			inputs_count++;
		else if (!counter[i].out && counter[i].in)
			outputs_count++;
	}
	size_t input_idx = 0, output_idx = 0;

	NetworkStream* inputs_steam = network_stream_create(FOURCC_INPUTS, sizeof(size_t), inputs_count);
	NetworkStream* outputs_steam = network_stream_create(FOURCC_OUTPUTS, sizeof(size_t), outputs_count);

	size_t* inputs_data = inputs_steam->data;
	size_t* outputs_data = outputs_steam->data;
	for (size_t i = 0; i < nodes_count; i++)
	{
		if (counter[i].out && !counter[i].in)
			inputs_data[input_idx++] = i;
		else if (!counter[i].out && counter[i].in)
			outputs_data[output_idx++] = i;
	}

	network_attach_stream(network, inputs_steam);
	network_attach_stream(network, outputs_steam);
}

void gexnet_process_links(Network* network)
{
	gexnet_process_links_weight(network, FOURCC_LINK_WEIGHT);
}

void gexnet_process_links_weight(Network* network, FourCC weight_stream)
{
	NetworkStream* nodes = network_get_stream_type(network, FOURCC_DATA);
	NetworkStream* links = network_get_stream_type(network, FOURCC_LINK);
	NetworkStream* weight = network_get_stream_type(network, weight_stream);

	if (!nodes || !links || !weight)
		return;

	NetworkLink* links_data = links->data;
	Number* node_data = nodes->data;
	Number* weight_data = weight->data;
	for (size_t i = 0; i < links->elementCount; i++)
	{
		node_data[links_data[i].output] += node_data[links_data[i].input] * weight_data[i];
	}
}

void gexnet_process_stream_add(Network* network, FourCC target, FourCC source)
{
	NetworkStream* target_stream = network_get_stream_type(network, target);
	NetworkStream* source_stream = network_get_stream_type(network, source);
	if (!target_stream || !source_stream || target_stream->elementCount > source_stream->elementCount)
		return;

	Number* target_data = target_stream->data;
	Number* source_data = source_stream->data;

	for (size_t i = 0; i < target_stream->elementCount; i++)
	{
		target_data[i] += source_data[i];
	}
}