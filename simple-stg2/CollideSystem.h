#pragma once

#include <vector>

#include "Collider.h"

class CollideAdt;


// 碰撞系统，该系统由GameSystem管理
class CollideSystem final
{
	friend class Collider;

public:

	static CollideSystem* instance();

	static void release_instance();

	CollideSystem();

	~CollideSystem();



	void update();


	int get_count_in_group(CollideGroup group);

	template<class Func>
	void foreach_in_group(CollideGroup group, Func&& func)
	{
		for (auto c : colliders[group])
			func(c);
	}


private:

	std::list<Collider*> colliders[(int)CollideGroup::collide_group_num];

	CollideAdt* adt[(int)CollideGroup::collide_group_num];

	int test(CollideGroup g1, CollideGroup g2);

};