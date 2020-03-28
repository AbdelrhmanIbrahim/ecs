#include <typeinfo>

#include "ecs/World.h"

struct Physics
{
	int p;
};

struct Mesh
{
	int m;
};

using namespace ecs;

int
main()
{
	auto w = world_new();
	auto e1 = world_entity_new(w);
	auto e2 = world_entity_new(w);
	auto e3 = world_entity_new(w);

	//problem for sure when component_storage reallocated its contents, do we need to pop up in each operation? 
	auto m1 = world_component_add<Mesh>(w, e1);
	auto m2 = world_component_add<Mesh>(w, e2);
	auto m3 = world_component_add<Mesh>(w, e3);
	auto daat = world_handle_component<Mesh>(w, m3);

	auto p1 = world_component_add<Physics>(w, e1);
	auto p2 = world_component_add<Physics>(w, e2);

	world_component_remove<Mesh>(w, e1);

	world_entity_remove(w, e1);

	return  0;
}
