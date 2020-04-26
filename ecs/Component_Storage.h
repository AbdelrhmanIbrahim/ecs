#pragma once

#include "Entity.h"
#include "Meta.h"

#include <vector>
#include <unordered_map>
#include <stdint.h>

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

	template<typename C>
	struct Component
	{
		Entity entity;
		C data;
		bool deleted;
	};

	template<typename T>
	struct
	Bag
	{
		int size;
		ecs::Component<T>* ptr;
	};

	struct Storage
	{
		virtual void
		entity_remove(Entity e) = 0;

		virtual void
		remove() = 0;
	};

	template<typename C>
	struct Component_Storage : Storage
	{
		std::vector<Entity> entities;
		std::vector<Component<C>> components;
		std::unordered_map<Entity, unsigned int, Entity_Hash> lookup;

		void
		entity_remove(Entity e)
		{
			storage_entity_remove<C>(this, e);
		}

		void
		remove()
		{
			for (auto& comp : components)
				if(comp.deleted == false)
					storage_entity_remove<C>(this, comp.entity);
		}
	};

	template <typename C>
	inline static Handle
	storage_entity_add(Component_Storage<C>* storage, Entity e)
	{
		Handle handle = storage_entity_comp_exists(storage, e);
		if (handle == INVALID_HANDLE)
		{
			storage->entities.push_back(e);
			storage->components.push_back(Component<C>{e, C{}, false});
			storage->lookup.insert(std::make_pair(e, storage->components.size() - 1));
			return Handle{ (int) storage->components.size() - 1};
		}
		else
			return handle;
	}

	template <typename C>
	inline static void
	storage_entity_remove(Component_Storage<C>* storage, Entity e)
	{
		auto pair = storage->lookup.find(e);
		if (pair != storage->lookup.end())
		{
			//mark as deleted
			size_t ix = pair->second;
			storage->entities[ix] = ecs::INVALID_ENTITY;
			storage->components[ix].entity = ecs::INVALID_ENTITY;
			storage->components[ix].deleted = true;
			storage->lookup.erase(e);
			if constexpr (has_free<C>::result)
				storage->components[ix].data.free();
		}
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
		if (storage->components[h.ix].deleted == false)
			return &storage->components[h.ix].data;

		return nullptr;
	}

	template<typename C>
	inline static ecs::Bag<C>
	storage_components_data(Component_Storage<C>* storage)
	{
		return ecs::Bag<C>{ (int)storage->components.size(), &storage->components.front() };
	}
};