#include "processors.h"
#include "memory.h"

#include <string.h>


typedef struct
{
	size_t in;
	size_t out;
} in_out_counter;

void gexnet_build_in_out_streams(Network* network)
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