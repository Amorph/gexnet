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

void network_stream_clear(NetworkStream* stream)
{
	NetworkStreamLockData* lock = network_stream_lock(stream, 0, 0);
	memset(lock->data, 0, stream->elementSize * lock->count);
	network_stream_unlock(lock);
}


NetworkStreamLockData*
network_stream_lock(NetworkStream* stream, size_t start_element, size_t count)
{
	if (start_element + count > stream->elementCount)
		return 0;

	if (!count)
		count = stream->elementCount - start_element;

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

void network_stream_set_indexed(NetworkStream* stream, NetworkStream* indexes, void* data, size_t start_index, size_t element_count)
{
	NetworkStreamLockData* target_lck = network_stream_lock(stream, 0, 0);
	NetworkStreamLockData* indexes_lck = network_stream_lock(indexes, start_index, element_count);

	size_t* indexes_data = indexes_lck->data;
	size_t	data_element_size = stream->elementSize;
	uint8_t* src_data = data;
	uint8_t* dst_data = target_lck->data;

	for (size_t i = 0; i < element_count; i++)
		memcpy(dst_data + indexes_data[i] * data_element_size, src_data + i * data_element_size, data_element_size);

	network_stream_unlock(indexes_lck);
	network_stream_unlock(target_lck);
}

void network_stream_get_indexed(NetworkStream* stream, NetworkStream* indexes, void* data, size_t start_index, size_t element_count)
{
	NetworkStreamLockData* target_lck = network_stream_lock(stream, 0, 0);
	NetworkStreamLockData* indexes_lck = network_stream_lock(indexes, start_index, element_count);

	size_t* indexes_data = indexes_lck->data;
	size_t	data_element_size = stream->elementSize;
	uint8_t* src_data = data;
	uint8_t* dst_data = target_lck->data;

	for (size_t i = 0; i < element_count; i++)
		memcpy(src_data + i * data_element_size, dst_data + indexes_data[i] * data_element_size, data_element_size);

	network_stream_unlock(indexes_lck);
	network_stream_unlock(target_lck);
}