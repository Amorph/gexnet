#include "network.h"
#include "memory.h"

#include <string.h>

Network* 
network_create()
{
	Network* net = allocator_get()->allocate(sizeof(Network));
	net->streams_count = 0;
	net->streams = 0;
	return net;
}

void 
network_destroy(Network* net)
{
	if (net->streams)
		allocator_get()->free(net->streams);
	allocator_get()->free(net);
}

size_t 
network_attach_stream(Network* net, NetworkStream* stream)
{
	if (!net->streams)
	{
		net->streams = allocator_get()->allocate(sizeof(net->streams));
		net->streams[0] = stream;
		net->streams_count = 1;
		return 0;
	}
	NetworkStream** old_streams = net->streams;
	net->streams = allocator_get()->allocate(sizeof(net->streams) * (net->streams_count + 1));
	for (int i = 0; i < net->streams_count; i++)
		net->streams[i] = old_streams[i];

	allocator_get()->free(old_streams);
	net->streams[net->streams_count] = stream;

	return net->streams_count++;
}

NetworkStream* 
network_get_stream_type(Network* net, FourCC type)
{
	for (size_t i = 0; i < net->streams_count; i++)
	{
		if (net->streams[i]->type == type)
			return net->streams[i];
	}
	return 0;
}

NetworkStream* 
network_stream_create(FourCC type, size_t element_size, size_t count)
{
	NetworkStream* stream = allocator_get()->allocate(sizeof(NetworkStream) + element_size * count);

	stream->data = stream + 1;
	stream->type = type;
	stream->elementSize = element_size;
	stream->elementCount = count;

	return stream;
}

void 
network_stream_destroy(NetworkStream* stream)
{
	allocator_get()->free(stream);
}

NetworkStreamLockData* 
network_stream_lock(NetworkStream* stream, size_t start_element, size_t count)
{
	if (start_element + count > stream->elementCount)
		return 0;

	NetworkStreamLockData* lock_data = allocator_get()->allocate(sizeof(NetworkStreamLockData));
	lock_data->stream = stream;
	lock_data->count = count;
	lock_data->data = ((uint8_t*)stream->data) + start_element * stream->elementSize;

	return lock_data;
}

void 
network_stream_unlock(NetworkStreamLockData* lock_data)
{
	allocator_get()->free(lock_data);
}