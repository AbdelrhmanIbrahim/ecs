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
	auto e4 = world_entity_new(w);

	auto h1 = world_component_add<Mesh>(w, e1);
	auto h2 = world_component_add<Mesh>(w, e2);
	auto h3 = world_component_add<Mesh>(w, e3);
	world_entity_remove(w, e1);

	return  0;
}
