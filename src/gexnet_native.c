#include "gexnet.h"

#include <stdlib.h>

struct GNSystemNative
{
	struct GNSystem pub;
};


static
void* mem_allocate(struct GNSystem* gn, size_t size, size_t flags)
{
	return malloc(size);
}

static
void mem_free(struct GNSystem* gn, void* mem)
{
	free(mem);
}


static struct _MemoryInterface
memory_interface = {
	.allocate = mem_allocate,
	.free = mem_free
};

static
struct GNStream* system_create_stream(struct GNSystem* gn, GNType type, GNIndex count, void* data)
{

}

static
void system_destroy(struct GNSystem* gn)
{
	gn->mem->free(gn, gn);
}

// --------------------------------------------------------------------------------------------------
static struct GNSystem
system_interface = {
	.mem = &memory_interface,
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
