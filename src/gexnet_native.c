#include "gexnet.h"
#include "stream.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>


struct GNSystemNative
{
	struct GNSystem		pub;
};
struct GNStreamNative
{
	struct GNStream		pub;
	void*				data;
	size_t				element_size;
};


static
void* mem_allocate(struct GNSystem* G, size_t size, size_t flags)
{
	return malloc(size);
}

static
void mem_free(struct GNSystem* G, void* mem)
{
	free(mem);
}


static struct _MemoryInterface
memory_interface = {
	.allocate = mem_allocate,
	.free = mem_free
};
// --------------------------------------------------------------------------------------------------
static size_t get_element_size(GNType type)
{
	switch (type)
	{
	case GN_TYPE_FLOAT:
		return sizeof(GNNumber);
	case GN_TYPE_INTEGER:
		return sizeof(GNInteger);
	case GN_TYPE_INDEX:
		return sizeof(GNIndex);
	case GN_TYPE_INDEX_INDEX:
		return get_element_size(GN_TYPE_INDEX) * 2;
	}
	return -1;
}

static void stream_destroy(struct GNStream* stream)
{
	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;
	struct GNSystem* G = stream->system;
	struct _MemoryInterface* mem = G->mem;

	mem->free(G, native_stream->data);
	mem->free(G, native_stream);
}

static struct GNStreamLockData* stream_lock(struct GNStream* stream, GNIndex start, GNIndex count, size_t flags)
{
	if (start + count > stream->count)
		return 0;

	if (!count)
		count = stream->count - start;

	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;
	struct GNSystem* G = stream->system;
	struct GNStreamLockData* lock_data = G->mem->allocate(G, sizeof(struct GNStreamLockData), 0);

	lock_data->stream = stream;
	lock_data->type = stream->type;
	lock_data->count = count;
	lock_data->data = ((uint8_t*)native_stream->data) + start * native_stream->element_size;
	
	return lock_data;
}

void stream_unlock(struct GNStreamLockData* lck)
{
	struct GNSystem* G = lck->stream->system;
	G->mem->free(G, lck);
}

static void stream_clear(struct GNStream* stream)
{
	if (stream->type != GN_TYPE_FLOAT && stream->type != GN_TYPE_INDEX && stream->type != GN_TYPE_INDEX_INDEX && stream->type != GN_TYPE_INTEGER)
		return;
	struct GNSystem* G = stream->system;
	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;

	struct GNStreamLockData* lock = G->stream->lock(stream, 0, 0, 0);
	memset(lock->data, 0, lock->count * native_stream->element_size);
	G->stream->unlock(lock);
}

static void stream_copy(struct GNStream* stream, struct GNStream* input)
{
	struct GNSystem* G = stream->system;
	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;


	struct GNStreamLockData* y_lock = G->stream->lock(stream, 0, 0, 0);
	struct GNStreamLockData* x_lock = G->stream->lock(input, 0, 0, 0);

	memcpy(y_lock->data, x_lock, native_stream->element_size * x_lock->count);

	G->stream->unlock(x_lock);
	G->stream->unlock(y_lock);
}

static void stream_set_stream_data(struct GNStream* stream, void* data, size_t count)
{
	struct GNSystem* G = stream->system;
	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;
	
	if (count > stream->count)
		count = stream->count;

	struct GNStreamLockData* lock = G->stream->lock(stream, 0, count, 0);
	memcpy(lock->data, data, count * native_stream->element_size);
	G->stream->unlock(lock);
}

static void stream_set_stream_data_indexed(struct GNStream* stream, struct GNStream* indexes, void* data)
{
	struct GNSystem* G = stream->system;
	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;

	struct GNStreamLockData* s_lock = G->stream->lock(stream, 0, 0, 0);
	struct GNStreamLockData* i_lock = G->stream->lock(indexes, 0, 0, 0);
	uint8_t* target_data = s_lock->data;
	uint8_t* source_data = data;
	GNIndex* index_data = i_lock->data;

	size_t es = native_stream->element_size;

	for (size_t i = 0; i < i_lock->count; i++)
		memcpy(target_data + es * index_data[i], source_data + es * i, es);

	G->stream->unlock(s_lock);
	G->stream->unlock(i_lock);
}

static void stream_get_stream_data_indexed(struct GNStream* stream, struct GNStream* indexes, struct GNStream* output)
{
	struct GNSystem* G = stream->system;
	struct GNStreamNative* native_stream = (struct GNStreamNative*)stream;

	if (indexes->type != GN_TYPE_INDEX)
		return;

	struct GNStreamLockData* s_lock = G->stream->lock(stream, 0, 0, 0);
	struct GNStreamLockData* i_lock = G->stream->lock(indexes, 0, 0, 0);
	struct GNStreamLockData* o_lock = G->stream->lock(output, 0, 0, 0);

	uint8_t* source_data = s_lock->data;
	uint8_t* target_data = o_lock->data;
	GNIndex* index_data = i_lock->data;

	size_t es = native_stream->element_size;

	for (size_t i = 0; i < i_lock->count; i++)
		memcpy(target_data + es * i, source_data + es * index_data[i], es);

	G->stream->unlock(s_lock);
	G->stream->unlock(i_lock);
	G->stream->unlock(o_lock);
}

static void stream_multiply_add_links(struct GNStream* stream, struct GNStream* links, struct GNStream* x, struct GNStream* weights)
{
	if (links->type != GN_TYPE_LINK || stream->type != GN_TYPE_NUMBER || x->type != GN_TYPE_NUMBER || weights->type != GN_TYPE_NUMBER)
		return;

	struct GNSystem* G = stream->system;

	struct GNStreamLockData* s_lock = G->stream->lock(stream, 0, 0, 0);
	struct GNStreamLockData* l_lock = G->stream->lock(links, 0, 0, 0);
	struct GNStreamLockData* x_lock = G->stream->lock(x, 0, 0, 0);
	struct GNStreamLockData* w_lock = G->stream->lock(weights, 0, 0, 0);

	GNNumber* s = s_lock->data;
	GNLink* l = l_lock->data;
	GNNumber* p = x_lock->data;
	GNNumber* w = w_lock->data;

	for (size_t i = 0; i < l_lock->count; i++)
		s[l[i].output] += p[l[i].input] * w[i];

	G->stream->unlock(s_lock);
	G->stream->unlock(l_lock);
	G->stream->unlock(x_lock);
	G->stream->unlock(w_lock);
}

static void stream_add(struct GNStream* stream, struct GNStream* input)
{
	if (stream->type != GN_TYPE_NUMBER || input->type != GN_TYPE_NUMBER)
		return;

	struct GNSystem* G = stream->system;

	struct GNStreamLockData* y_lock = G->stream->lock(stream, 0, 0, 0);
	struct GNStreamLockData* x_lock = G->stream->lock(input, 0, 0, 0);

	GNNumber* y = y_lock->data;
	GNNumber* x = x_lock->data;

	for (size_t i = 0; i < y_lock->count; i++)
	{
		y[i] += x[i];
	}
	G->stream->unlock(x_lock);
	G->stream->unlock(y_lock);
}

static GNNumber _processing_func_tanh(GNNumber x)
{
	return (GNNumber)((exp(2 * x) - 1) / (exp(2 * x) + 1));
}

static void stream_process_stream(struct GNStream* stream, struct GNStream* input, size_t function)
{
	if (stream->type != GN_TYPE_NUMBER || input->type != GN_TYPE_NUMBER)
		return;

	if (function != GN_FUNCTION_TANH)
		return;

	struct GNSystem* G = stream->system;

	struct GNStreamLockData* y_lock = G->stream->lock(stream, 0, 0, 0);
	struct GNStreamLockData* x_lock = G->stream->lock(input, 0, 0, 0);

	GNNumber* y = y_lock->data;
	GNNumber* x = x_lock->data;

	for (size_t i = 0; i < y_lock->count; i++)
	{
		y[i] = _processing_func_tanh(x[i]);
	}

	G->stream->unlock(x_lock);
	G->stream->unlock(y_lock);
}

static struct _StreamInterface stream_interface =
{
	.destroy = stream_destroy,
	.lock = stream_lock,
	.unlock = stream_unlock,
	.clear = stream_clear,
	.copy = stream_copy,
	.set_stream_data = stream_set_stream_data,
	.set_stream_data_indexed = stream_set_stream_data_indexed,
	.get_stream_data_indexed = stream_get_stream_data_indexed,
	.multiply_add_links = stream_multiply_add_links,
	.add = stream_add,
	.process_stream = stream_process_stream
};

// --------------------------------------------------------------------------------------------------
static
struct GNStream* system_create_stream(struct GNSystem* G, GNType type, GNIndex count, void* data)
{
	struct GNStreamNative* stream = G->mem->allocate(G, sizeof(struct GNStreamNative), 0);

	stream->pub.system = G;
	stream->pub.type = type;
	stream->pub.count = count;

	size_t element_size = get_element_size(type);
	size_t buffer_size = element_size * count;

	stream->element_size = element_size;
	stream->data = G->mem->allocate(G, buffer_size, 0);
	if(data)
		memcpy(stream->data, data, buffer_size);
	
	return (struct GNStream*)stream;
}

static
void system_destroy(struct GNSystem* G)
{
	G->mem->free(G, G);
}

// --------------------------------------------------------------------------------------------------
static struct GNSystem
system_interface = {
	.mem = &memory_interface,
	.stream = &stream_interface,
	.create_stream = system_create_stream,
	.destroy = system_destroy
};

// --------------------------------------------------------------------------------------------------
struct GNSystem* gexnet_native_init(void* init_data)
{
	struct GNSystemNative* system = memory_interface.allocate(0, sizeof(struct GNSystemNative), 0);

	system->pub = system_interface;
	return (struct GNSystem*)system;
}
