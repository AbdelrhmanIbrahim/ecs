#include "ecs/World.h"
#include "ecs/Meta.h"
using namespace ecs;

struct Physics
{
	int p;
};

struct Mesh
{
	int m;
	void
	free()
	{
		m = 100000;
	}
};

int
main()
{
	auto w = world_new();
	auto e0 = world_entity_new(w);
	auto e1 = world_entity_new(w);
	auto e2 = world_entity_new(w);
	auto e3 = world_entity_new(w);

	auto h0 = world_component_add<Mesh>(w, e0);
	auto h1 = world_component_add<Mesh>(w, e1);
	auto h2 = world_component_add<Mesh>(w, e2);
	auto h3 = world_component_add<Mesh>(w, e3);

	world_handle_component<Mesh>(w, h0)->m = 1;
	world_handle_component<Mesh>(w, h1)->m = 2;
	world_handle_component<Mesh>(w, h2)->m = 3;
	world_handle_component<Mesh>(w, h3)->m = 4;

	auto bag = world_active_components<Mesh>(w);
	world_free(w);
	
	return  0;
}
