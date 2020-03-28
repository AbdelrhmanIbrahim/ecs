#pragma once

#include "Entity.h"

#include <vector>
#include <unordered_map>
#include <stdint.h>

namespace ecs
{
	struct Storage
	{
		virtual void*
		entity_add(Entity e) = 0;

		virtual void
		entity_remove(Entity e) = 0;

		virtual void*
		entity_comp(Entity e) = 0;
	};

	template<typename C>
	struct Component_Storage : Storage
	{
		std::vector<Entity> entities;
		std::vector<C> components;
		std::unordered_map<Entity, unsigned int, Entity_Hash> lookup;

		void*
		entity_add(Entity e) override
		{
			return storage_entity_add<C>(*this, e);
		}

		void
		entity_remove(Entity e) override
		{
			storage_entity_remove<C>(*this, e);
		}

		void*
		entity_comp(Entity e) override
		{
			return storage_entity_comp_exists<C>(*this, e);
		}
	};

	template <typename Component>
	inline static Component*
	storage_entity_add(Component_Storage<Component>& storage, Entity e)
	{
		Component* comp = storage_entity_comp_exists(storage, e);
		if (comp == nullptr)
		{
			storage.entities.push_back(e);
			storage.components.push_back(Component{});
			storage.lookup.insert(std::make_pair(e, storage.components.size() - 1));

			//problametic, way to keep with polymorphism of base fn, return handle instead
			return &storage.components.back();
		}
		else
			return comp;
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
			storage.entities[ix] = storage.entities.back();
			storage.components[ix] = std::move(storage.components.back());
			storage.lookup[storage.entities[ix]] = ix;

			//shrink
			storage.entities.pop_back();
			storage.components.pop_back();
			storage.lookup.erase(e);
		}
	}

	template <typename Component>
	inline static Component*
	storage_entity_comp_exists(Component_Storage<Component>& storage, Entity e)
	{
		auto pair = storage.lookup.find(e);
		if (pair != storage.lookup.end())
			return &storage.components[pair->second];

		return nullptr;
	}
};