#pragma once

#include <stdint.h>

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

	static constexpr Entity INVALID_ENTITY{ 0 };
};