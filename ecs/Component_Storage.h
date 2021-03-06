#pragma once

#include "Entity.h"
#include "Meta.h"

#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <algorithm>

namespace ecs
{
	struct Handle
	{
		int ix;

		bool
		operator==(const Handle& other) const
		{
			return  ix == other.ix;
		}

		bool
		operator!=(const Handle& other) const
		{
			return  ix != other.ix;
		}
	};
	static constexpr Handle INVALID_HANDLE{ -1 };

	template<typename T>
	struct
	Bag
	{
		int size;
		T* ptr;

		T&
		operator[](unsigned int index)
		{
			return ptr[index];
		}
	};

	struct Storage
	{
		virtual void
		entity_remove(Entity e) = 0;

		virtual void
		free() = 0;
	};

	template<typename C>
	struct Component_Storage : Storage
	{
		int active_components;
		std::vector<Entity> entities;
		std::vector<C> components;
		std::unordered_map<Entity, unsigned int, Entity_Hash> lookup; //<entity, component_index>

		void
		entity_remove(Entity e)
		{
			//removal happens by  marking as deleted and swapping it with the first active element in both arrays
			//to have a contionus active data (better caching and no branching needed)
			auto pair = lookup.find(e);
			if (pair != lookup.end())
			{
				size_t to_delete_ix = pair->second;
				entities[to_delete_ix] = ecs::INVALID_ENTITY;

				size_t first_active_ix = entities.size() - active_components;
				lookup[entities[first_active_ix]] = to_delete_ix;
				std::swap(entities[first_active_ix], entities[to_delete_ix]);
				std::swap(components[first_active_ix], components[to_delete_ix]);

				active_components--;
				lookup.erase(e);
				if constexpr (has_free<C>::result)
					components[first_active_ix].free();
			}
		}

		void
		free()
		{
			int first_active_ix = entities.size() - active_components;
			int size = first_active_ix + active_components;
			for (int i = first_active_ix ; i < size; ++i)
				entity_remove(entities[i]);
		}
	};

	template <typename C>
	inline static Handle
	storage_entity_add(Component_Storage<C>* storage, Entity e)
	{
		Handle handle = storage_entity_comp_exists(storage, e);
		if (handle == INVALID_HANDLE)
		{
			storage->active_components++;
			storage->entities.emplace_back(e);
			storage->components.emplace_back(C{});
			storage->lookup.insert(std::make_pair(e, storage->components.size() - 1));
			return Handle{ (int) storage->components.size() - 1};
		}
		else
			return handle;
	}

	template <typename C>
	inline static Handle
	storage_entity_comp_exists(Component_Storage<C>* storage, Entity e)
	{
		auto pair = storage->lookup.find(e);
		if (pair != storage->lookup.end())
			return Handle{ (int) pair->second };

		return INVALID_HANDLE;
	}

	template<typename C>
	inline static C*
	storage_handle_component(Component_Storage<C>* storage, Handle h)
	{
		if (h.ix >= storage->entities.size() - storage->active_components)
			return &storage->components[h.ix];

		return nullptr;
	}

	template<typename C>
	inline static ecs::Bag<C>
	storage_active_components(Component_Storage<C>* storage)
	{
		return ecs::Bag<C>{ (int)storage->active_components, &storage->components[storage->components.size() - storage->active_components] };
	}

	template<typename C>
	inline static ecs::Bag<Entity>
	storage_active_entities(Component_Storage<C>* storage)
	{
		return ecs::Bag<Entity>{ (int)storage->active_components, &storage->entities[storage->entities.size() - storage->active_components] };
	}
};