#include "CollideTileMap.h"
#include <cmath>

namespace
{
	constexpr double eps = 1e-8;
}

CollideTileMap::CollideTileMap(const AABB& area, double tile_w, double tile_h)
	: area(area), tile_w(tile_w), tile_h(tile_h)
{
	w = std::ceil(area.w() / tile_w + eps);
	h = std::ceil(area.h() / tile_h + eps);
	tiles.resize(w * h);
}

CollideTileMap::~CollideTileMap()
{

}

void CollideTileMap::insert(Collider* c)
{
	auto r = get_range(c->get_aabb());
	for (int x = r.x1; x <= r.x2; x++)
		for (int y = r.y1; y <= r.y2; y++)
			tile(x, y).push_back(c);
}

void CollideTileMap::build()
{

}

int CollideTileMap::test(Collider* c)
{
	int fail = 0;
	auto r = get_range(c->get_aabb());
	for (int x = r.x1; x <= r.x2; x++)
		for (int y = r.y1; y <= r.y2; y++)
			for (auto c2 : tile(x, y))
				fail += !test_collide(c, c2);
	return fail;
}

void CollideTileMap::clear()
{
	for (auto& tile : tiles)
		tile.clear();
}

CollideTileMap::Range CollideTileMap::get_range(const AABB& aabb) const
{
	Range r;
	r.x1 = std::floor((aabb.x1 - area.x1) / tile_w - eps);
	r.y1 = std::floor((aabb.y1 - area.y1) / tile_h - eps);
	r.x2 = std::floor((aabb.x2 - area.x1) / tile_w + eps);
	r.y2 = std::floor((aabb.y2 - area.y1) / tile_h + eps);
	r.x1 = std::max(0, std::min(w - 1, r.x1));
	r.y1 = std::max(0, std::min(h - 1, r.y1));
	r.x2 = std::max(0, std::min(w - 1, r.x2));
	r.y2 = std::max(0, std::min(h - 1, r.y2));
	return r;
}

CollideTileMap::Tile& CollideTileMap::tile(int x, int y)
{
	return tiles[x + y * w];
}

const CollideTileMap::Tile& CollideTileMap::tile(int x, int y) const
{
	return tiles[x + y * w];
}
