#pragma once

#include "Entity.h"
#include "Component_Storage.h"

#include <vector>
#include <unordered_map>
#include <vcruntime.h>
#include <utility>
#include <typeinfo>

namespace ecs
{
	//no garbage collection, we don't actually deleteing when remove, we just marking things as deleted to simplify indexing and data access later
	//make garbage collection if needed -revisit-

	typedef size_t Component_Type_Hash;
	struct World
	{
		unsigned int id_counter;
		std::vector<Entity> entities;
		std::unordered_map<Component_Type_Hash, void*> type_storage_map;
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
		w.entities.push_back(Entity{ ++w.id_counter });
		return w.entities.back();
	}

	inline static void
	world_entity_remove(World& w, Entity e)
	{
		//mark as deleted
		w.entities[e.id - 1] = INVALID_ENTITY;

		//now mark as deleteds from components storage
		for (auto it = w.type_storage_map.begin(); it != w.type_storage_map.end(); ++it)
			((Storage*)it->second)->entity_remove(e);
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
			void* storage = pair->second;
			return storage_entity_add(((Component_Storage<C>*)storage), e);
		}
		else
		{
			//fragmented..custom stack allocater if performance is bad -revisit-
			void* storage = (void*) new Component_Storage<C>;
			w.type_storage_map.insert(std::make_pair(type_hash, storage));
			return storage_entity_add(((Component_Storage<C>*)storage), e);
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
			((Component_Storage<C>*)(pair->second))->entity_remove(e);
	}

	template<typename C>
	inline static C*
	world_handle_component(World& w, Handle h)
	{
		C dummy;
		Component_Type_Hash type_hash = typeid(dummy).hash_code();
		auto pair = w.type_storage_map.find(type_hash);
		if (pair != w.type_storage_map.end())
			return storage_handle_component((Component_Storage<C>*)(pair->second), h);

		return nullptr;
	}

	//always use ref variable if you want to edit data and check if component marked as deleted or not
	template<typename C>
	inline static std::vector<Component<C>>&
	world_components_data(World& w)
	{
		C dummy;
		Component_Type_Hash type_hash = typeid(dummy).hash_code();
		auto pair = w.type_storage_map.find(type_hash);
		if (pair != w.type_storage_map.end())
			return storage_components_data(((Component_Storage<C>*)(pair->second)));

		return std::vector<Component<C>>{};
	}
};