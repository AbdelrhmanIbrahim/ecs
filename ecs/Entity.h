#pragma once

#include <stdint.h>
#include <type_traits>
#include <cstddef>
#include <functional>

namespace ecs
{
	struct Entity
	{
		uint32_t id;

		bool
		operator==(const Entity& other) const
		{
			return  id == other.id;
		}

		bool
		operator!=(const Entity& other) const
		{
			return  id != other.id;
		}
	};

	struct Entity_Hash
	{
		size_t
			operator()(const Entity& e) const
		{
			return std::hash<uint32_t>()(e.id);
		}
	};

	static constexpr Entity INVALID_ENTITY{ 0 };
};