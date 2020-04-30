#include "ecs/World.h"
#include "ecs/Meta.h"
#include "ecs/Script.h"

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

void
some_script(Mesh* c1, Mesh* c2)
{
	c1->m = c2->m + 4;
}

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
	world_handle_component<Mesh>(w, h0)->m = 4;

	world_entity_remove(w, e3);
	auto es = world_active_entities<Mesh>(w);
	auto cs = world_active_components<Mesh>(w);

	//some scripts
	ecs::Script<Mesh, Mesh> sc{ e0, e1, some_script };

	//scripts system will run all scripts later at the end of the update fn
	script_run(sc, w);

	world_free(w);

	return  0;
}
