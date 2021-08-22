#include "CollideSystem.h"
#include "GameSystem.h"

#include "CollideBvhTree.h"
#include "CollideTileMap.h"

namespace
{
	CollideSystem* instance = nullptr;
}

CollideSystem* CollideSystem::instance()
{
	if (!::instance)
	{
		::instance = new CollideSystem;
	}
	return ::instance;
}

void CollideSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}


CollideSystem::CollideSystem()
{
	for (int i = 0; i < (int)CollideGroup::collide_group_num; i++)
		 adt[i] = new CollideBvhTree(20);
		// adt[i] = new CollideTileMap({ -50, -50, GameSystem::game_w + 50, GameSystem::game_h + 50 }, 10, 10);
}

CollideSystem::~CollideSystem()
{
	for (int i = 0; i < (int)CollideGroup::collide_group_num; i++)
		delete adt[i];
}


void CollideSystem::update()
{
	for (int i = 0; i < (int)CollideGroup::collide_group_num; i++)
	{
		adt[i]->clear();
		for (auto c : colliders[i])
			adt[i]->insert(c);
		adt[i]->build();
	}

	int failed_count = 0;
	for (int i = 1; i < (int)CollideGroup::collide_group_num; i++)
		failed_count += test(CollideGroup::all, (CollideGroup)i);
	failed_count += test(CollideGroup::player,			CollideGroup::enemy);
	failed_count += test(CollideGroup::player,			CollideGroup::enemy_bullet);
	failed_count += test(CollideGroup::player,			CollideGroup::item);
	failed_count += test(CollideGroup::enemy,			CollideGroup::player_bullet);
	failed_count += test(CollideGroup::enemy,			CollideGroup::player_bullet_bomb);
	failed_count += test(CollideGroup::enemy_bullet,	CollideGroup::player_bullet_bomb);
}

int CollideSystem::get_count_in_group(CollideGroup group)
{
	return colliders[(int)group].size();
}

int CollideSystem::test(CollideGroup g1, CollideGroup g2)
{
	if (colliders[(int)g1].size() > colliders[(int)g2].size())
		std::swap(g1, g2);
	int fail = 0;
	for (auto c : colliders[(int)g1])
		fail += adt[(int)g2]->test(c);
	return fail;
}
