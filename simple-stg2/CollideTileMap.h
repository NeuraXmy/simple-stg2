#pragma once

#include <vector>
#include "CollideAdt.h"

class CollideTileMap final : public CollideAdt
{
	using Tile = std::vector<Collider*>;

public:

	CollideTileMap(const AABB& area, double tile_w, double tile_h);

	~CollideTileMap();

	void insert(Collider* c) override;

	void build() override;

	int test(Collider* c) override;

	void clear() override;

private:

	AABB area;
	int w, h;
	double tile_w, tile_h;
	std::vector<std::vector<Collider*>> tiles;

	struct Range { int x1, y1, x2, y2; };


	Tile& tile(int x, int y);

	const Tile& tile(int x, int y) const;

	Range get_range(const AABB& aabb) const;

};
