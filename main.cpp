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
		m++;
	}
};

int
main()
{
	auto w = world_new();
	auto e0 = world_entity_new(w);
	auto e1 = world_entity_new(w);
	auto e2 = world_entity_new(w);

	auto h0 = world_component_add<Mesh>(w, e0);
	auto h1 = world_component_add<Mesh>(w, e1);
	auto h2 = world_component_add<Mesh>(w, e2);

	world_handle_component<Mesh>(w, h0)->m = 4;
	world_handle_component<Mesh>(w, h1)->m = 5;

	auto bag = world_components_data<Mesh>(w);
	//bag.ptr[2].data.m = 6;

	std::vector<Mesh> m;
	for (int x = 0; x < bag.size; ++x)
		m.push_back(bag.ptr[x].data);

	//world_entity_remove(w, e1);
	//world_entity_remove(w, e2);

	world_free(w);
	return  0;
}
