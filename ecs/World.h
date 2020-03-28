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
	struct World
	{
		unsigned int id_counter;
		std::vector<Entity> entities;
		std::unordered_map<Component_Type_Hash, ecs::Storage*> type_storage_map;
	};

	inline static World
	world_new()
	{
		World w{};
		return w;
	}

	inline static void
	world_free(World& w)
	{
		for (auto it = w.type_storage_map.begin(); it != w.type_storage_map.end(); ++it)
			delete it->second;
	}

	inline static Entity
	world_entity_new(World& w)
	{
		w.id_counter++;
		w.entities.push_back(Entity{ w.id_counter });
		return w.entities.back();
	}

	inline static void
	world_entity_remove(World& w, Entity e)
	{
		//remove from universe entities
		//for now, later make hashmap(Entity -> Index in entities vector)
		for (unsigned int ix = 0; ix < w.entities.size(); ++ix)
		{
			if (w.entities[ix] == e)
			{
				w.entities[ix] = w.entities.back();
				w.entities.pop_back();
				break;
			}
		}

		//now remove it from components storage
		for (auto it = w.type_storage_map.begin(); it != w.type_storage_map.end(); ++it)
			it->second->entity_remove(e);
	}

	template <typename C>
	inline static Handle
	world_component_add(World& w, Entity e)
	{
		C dummy;
		Component_Type_Hash type_hash = typeid(dummy).hash_code();
		auto pair = w.type_storage_map.find(type_hash);
		if (pair != w.type_storage_map.end())
		{
			Storage* storage = pair->second;
			return storage->entity_add(e);
		}
		else
		{
			//fragmented..custom stack allocater if performance is bad -revisit-
			Storage* storage = new Component_Storage<C>;
			w.type_storage_map.insert(std::make_pair(type_hash, storage));
			return storage->entity_add(e);
		}
	}

	template <typename C>
	inline static void
	world_component_remove(World& w, Entity e)
	{
		C dummy;
		Component_Type_Hash type_hash = typeid(dummy).hash_code();
		auto pair = w.type_storage_map.find(type_hash);
		if (pair != w.type_storage_map.end())
			pair->second->entity_remove(e);
	}

	template<typename C>
	inline static C*
	world_handle_component(World& w, Handle h)
	{
		C dummy;
		Component_Type_Hash type_hash = typeid(dummy).hash_code();
		auto pair = w.type_storage_map.find(type_hash);
		if (pair != w.type_storage_map.end())
			return (C*)(pair->second->handle_component(h));

		return nullptr;
	}
};