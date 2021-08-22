#pragma once

#include <vector>
#include "CollideAdt.h"

class CollideBvhTree final : public CollideAdt
{
public:

	CollideBvhTree(int split_count);

	~CollideBvhTree();

	void insert(Collider* c) override;

	void build() override;

	int test(Collider* c) override;

	void clear() override;


	int get_split_count() const;

	int get_node_count() const;

private:

	struct Node
	{
		AABB aabb = { 1e9, 1e9, 1e-9, 1e-9 };
		Node* child[2] = {};
		Node* father = nullptr;
		bool vertical = false;
		int dep = 0;
		int l = 0, r = 0;
	};

	Node* root = nullptr;

	int node_count = 1;

	int split_count = 20;


	struct CollideAABB
	{
		Collider* collider;
		AABB aabb;
	};
	std::vector<CollideAABB> colliders;
	int colliders_top = 0;

};
