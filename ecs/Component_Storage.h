#pragma once

#include "Entity.h"

#include <vector>
#include <unordered_map>
#include <stdint.h>

namespace ecs
{
	struct Entity_Hash
	{
		size_t
		operator()(const Entity& e) const
		{
			return std::hash<uint32_t>()(e.id);
		}
	};

	template<typename C>
	struct Component_Storage
	{
		std::vector<Entity> ientities;
		std::vector<C> icomponents;
		std::unordered_map<Entity, unsigned int, Entity_Hash> lookup;
	};

	template <typename Component>
	inline static Component&
	storage_entity_add(Component_Storage<Component>& storage, Entity e)
	{
		storage.ientities.push_back(e);
		storage.icomponents.push_back(Component{});
		storage.lookup.insert(std::make_pair(e, storage.icomponents.size()-1));

		return storage.icomponents.back();
	}

	template <typename Component>
	inline static void
	storage_entity_remove(Component_Storage<Component>& storage, Entity e)
	{
		auto pair = storage.lookup.find(e);
		if (pair != storage.lookup.end())
		{
			const size_t ix = pair->second;

			//swap with last
			storage.ientities[ix] = storage.ientities.back();
			storage.icomponents[ix] = std::move(storage.icomponents.back());
			storage.lookup[storage.ientities[ix]] = ix;

			//shrink
			storage.ientities.pop_back();
			storage.icomponents.pop_back();
			storage.lookup.erase(e);
		}
	}

	template <typename Component>
	inline static Component*
	storage_entity_comp(Component_Storage<Component>& storage, Entity e)
	{
		auto pair = storage.lookup.find(e);
		if (pair != storage.lookup.end())
			return &storage.icomponents[pair->second];

		return nullptr;
	}
};