#include "processors.h"
#include "memory.h"

#include <string.h>


typedef struct
{
	size_t in;
	size_t out;
} in_out_counter;

size_t gexnet_compute_node_count(NetworkStream* links)
{
	size_t max_index = 0;

	if (!links || !links->elementCount)
		return 0;
	
	NetworkStreamLockData* links_stream = network_stream_lock(links, 0, 0);

	if (!links_stream)
		return 0;

	size_t left = links_stream->count;
	if (!left)
		return 0;

	NetworkLink* links_data = (NetworkLink*)links->data;
	while (left--)
	{
		if (max_index < links_data->input)
			max_index = links_data->input;
		if (max_index < links_data->output)
			max_index = links_data->output;
		links_data++;
	}

	network_stream_unlock(links_stream);

	return max_index + 1;
}

bool gexnet_compute_in_out_streams(NetworkStream* links, size_t node_count, NetworkStream** inputs, NetworkStream** outputs)
{
	if (!links)
		return false;

	if (!node_count)
		node_count = gexnet_compute_node_count(links);

	NetworkLink* links_data = (NetworkLink*)links->data;

	in_out_counter* counter = allocator_get()->allocate(sizeof(in_out_counter) * node_count);
	memset(counter, 0, sizeof(in_out_counter) * node_count);

	for (size_t i = 0; i < links->elementCount; i++)
	{
		counter[links_data[i].input].out++;
		counter[links_data[i].output].in++;
	}

	size_t inputs_count = 0, outputs_count = 0;
	for (size_t i = 0; i < node_count; i++)
	{
		if (counter[i].out && !counter[i].in)
			inputs_count++;
		else if (!counter[i].out && counter[i].in)
			outputs_count++;
	}
	if (!inputs_count || !outputs_count)
	{
		allocator_get()->free(counter);
		return false;
	}
	size_t input_idx = 0, output_idx = 0;

	NetworkStream* inputs_steam = 0;
	NetworkStream* outputs_steam = 0;

	if(inputs)
		inputs_steam = network_stream_create(FOURCC_INPUTS, sizeof(size_t), inputs_count);
	if(outputs)
		outputs_steam = network_stream_create(FOURCC_OUTPUTS, sizeof(size_t), outputs_count);

	size_t* inputs_data = inputs_steam ? inputs_steam->data : 0;
	size_t* outputs_data = outputs_steam ? outputs_steam->data : 0;
	for (size_t i = 0; i < node_count; i++)
	{
		if (inputs_data && counter[i].out && !counter[i].in)
			inputs_data[input_idx++] = i;
		else if (outputs_data && !counter[i].out && counter[i].in)
			outputs_data[output_idx++] = i;
	}

	allocator_get()->free(counter);

	if(inputs)
		*inputs = inputs_steam;

	if(outputs)
		*outputs = outputs_steam;

	return true;
}

void gexnet_process_links_weight(NetworkStream* links, NetworkStream* weight, NetworkStream* output)
{
	if (!output || !links || !weight)
		return;

	NetworkLink* links_data = links->data;
	Number* node_data = output->data;
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
