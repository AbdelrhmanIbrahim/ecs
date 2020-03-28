#include <typeinfo>

#include "ecs/Component_Storage.h"

struct phy
{
	int x, y;
};


struct greater_stuff
{
	virtual void lol() = 0;
};

template<typename L>
void neh()
{
	int x = 3;
}
template<typename C>
struct stuff : public greater_stuff
{
	int x;

	void lol() override
	{
		neh<C>();
	};
};

struct tes
{
	int x, y;
};
int
main()
{
	ecs::Storage* a = new ecs::Component_Storage<tes>;
	a->entity_add(ecs::Entity{ 2 });
	auto s = (tes*)a->entity_comp(ecs::Entity{ 2 });

	greater_stuff* h = new stuff<int>;
	h->lol();

	ecs::Component_Storage<phy> phy_storage;
	ecs::storage_entity_add(phy_storage, ecs::Entity{ 5 });
	ecs::storage_entity_add(phy_storage, ecs::Entity{ 8 });
	ecs::storage_entity_add(phy_storage, ecs::Entity{ 9 });
	auto comp_2 = ecs::storage_entity_comp(phy_storage, ecs::Entity{ 8 });
	//::storage_entity_remove(phy_storage, ecs::Entity{ 8 });

	return  0;
}
