#pragma once

#include "Entity.h"
#include "Component_Storage.h"

#include <vector>
#include <unordered_map>
#include <vcruntime.h>
#include <utility>

namespace ecs
{
	typedef size_t Component_Type_Hash;
	struct Universe
	{
		unsigned int id_counter;
		std::vector<Entity> entities;
		std::unordered_map<Component_Type_Hash, ecs::Storage*> type_storage_map;
	};

	inline static Entity
	universe_entity_new(Universe& u)
	{
		u.id_counter++;
		u.entities.push_back(Entity{ u.id_counter });
		return u.entities.back();
	}

	inline static void
	universe_entity_remove(Universe& u, Entity e)
	{
		//remove from universe entities
		//for now, later make hashmap(Entity -> Index in entities vector)
		for (unsigned int ix = 0; ix < u.entities.size(); ++ix)
		{
			if (u.entities[ix] == e)
			{
				u.entities[ix] = u.entities.back();
				break;
			}
		}
		u.entities.pop_back();

		//now remove it from components storage
		for (auto it = u.type_storage_map.begin(); it != u.type_storage_map.end(); ++it)
			it->second->entity_remove(e);
	}

	template <typename C>
	inline static C*
	universe_component_add(Universe& u, Entity e)
	{
		C dummy;
		Component_Type_Hash type_hash = typeid(dummy).hash_code();
		auto pair = u.type_storage_map.find(type_hash);
		if (pair != u.type_storage_map.end())
		{
			Storage* storage = pair->second;
			return (C*)storage->entity_add(e);
		}
		else
		{
			Storage* storage = new Component_Storage<C>;
			u.type_storage_map.insert(std::make_pair(type_hash, storage));
			return (C*)storage->entity_add(e);
		}
	}

	template <typename C>
	inline static void
	universe_component_remove(Universe& u, Entity e)
	{

	}
};