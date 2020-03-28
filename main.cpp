#include <typeinfo>

#include "ecs/Component_Storage.h"

struct phy
{
	int x, y;
};
int
main()
{
	ecs::Component_Storage<phy> phy_storage;
	ecs::storage_entity_add(phy_storage, ecs::Entity{ 5 });
	ecs::storage_entity_add(phy_storage, ecs::Entity{ 8 });
	ecs::storage_entity_add(phy_storage, ecs::Entity{ 9 });
	auto comp_2 = ecs::storage_entity_comp(phy_storage, ecs::Entity{ 8 });
	ecs::storage_entity_remove(phy_storage, ecs::Entity{ 8 });

	auto s = typeid(int).hash_code();
	return  0;
}