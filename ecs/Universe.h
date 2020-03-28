#pragma once

#include "Entity.h"

#include <vector>
#include <unordered_map>

namespace ecs
{
	typedef unsigned int Component_Type_Hash;
	struct Universe
	{
		std::vector<Entity> entities;
		std::unordered_map<Component_Type_Hash, void*> comp_storage; //void* = Component_Storage<Component_Type>*
	};

	inline static Entity
	universe_entity_new(Universe& u)
	{
		u.entities.push_back(Entity{ u.entities.size() + 1 });
		return u.entities.back();
	}

	inline static void
	universe_entity_remove(Universe& u, Entity e)
	{

	}

	template <typename C>
	inline static C*
	universe_component_add(Universe& u, Entity e)
	{

	}

	template <typename C>
	inline static void
	universe_component_remove(Universe& u, Entity e)
	{

	}
};