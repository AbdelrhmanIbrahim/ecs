#pragma once

#include "Entity.h"
#include "World.h"

namespace ecs
{
	template<typename C1, typename C2>
	struct Script
	{
		Entity e1;
		Entity e2;
		void (*fn)(C1*, C2*);
	};

	template<typename C1, typename C2>
	inline static void
	script_run(Script<C1, C2> script, World& w)
	{
		C1* c1 = world_handle_component<C1>(w, world_component_add<C1>(w, script.e1));
		C2* c2 = world_handle_component<C2>(w, world_component_add<C2>(w, script.e2));
		script.fn(c1, c2);
	}
};