#include "PItem.h"
#include "PResource.h"
#include "Player.h"
#include "GameSystem.h"
#include "Sound.h"

void SmallItem::on_create()
{
	Item::on_create();
	set_pick_radius(50.0);
	set_start_info(-2.5, 30, true, true);
	set_drop_info(2.0, 0.05, 30);
	set_pick_se(se.item_pick);
}

void BigItem::on_create() 
{
	Item::on_create();
	sprite().set_level(DrawLevel::item_2);
	set_pick_radius(80.0);
	set_start_info(-2.5, 30, true, true);
	set_drop_info(2.0, 0.05, 30);
	set_pick_se(se.item_pick);
}

void PowerItem::on_create()
{
	SmallItem::on_create();
	sprite().set_tex_and_coords(tex.item, coords.item_power);
	indicator().set_tex_and_coords(tex.item, coords.item_power_ind);
}
void PowerItem::on_pick(Player* player)
{
	SmallItem::on_pick(player);
	if (player->is_full_power())
		player->add_score(1000);
	else
		player->add_power(1);
}

void PointItem::on_create() 
{
	SmallItem::on_create();
	sprite().set_tex_and_coords(tex.item, coords.item_point);
	indicator().set_tex_and_coords(tex.item, coords.item_point_ind);
}
void PointItem::on_pick(Player* player) 
{
	SmallItem::on_pick(player);
	double k = utils::clamp(1.5 - player->y() / GameSystem::game_h, 0.0, 1.0);
	player->add_score(player->get_point_score() * k);
}

void PowerBigItem::on_create() 
{
	BigItem::on_create();
	sprite().set_tex_and_coords(tex.item, coords.item_power_big);
	sprite().set_scale(0.8, 0.8);
	indicator().set_tex_and_coords(tex.item, coords.item_power_big_ind);
}
void PowerBigItem::on_pick(Player* player)
{
	BigItem::on_pick(player);
	if (player->is_full_power())
		player->add_score(500);
	else
		player->add_power(50);
}

void FullItem::on_create() 
{
	BigItem::on_create();
	sprite().set_tex_and_coords(tex.item, coords.item_full);
	sprite().set_scale(0.8, 0.8);
	indicator().set_tex_and_coords(tex.item, coords.item_full_ind);
}
void FullItem::on_pick(Player* player) 
{
	BigItem::on_pick(player);
	if (player->is_full_power())
		player->add_score(10000);
	else
		player->set_power(player->get_max_power());
}

void LifeItem::on_create()
{
	BigItem::on_create();
	sprite().set_tex_and_coords(tex.item, coords.item_life);
	sprite().set_scale(1.5, 1.5);
	indicator().set_tex_and_coords(tex.item, coords.item_life_ind);
}
void LifeItem::on_pick(Player* player)
{
	player->extend_life();
}

void BombItem::on_create() 
{
	BigItem::on_create();
	sprite().set_tex_and_coords(tex.item, coords.item_bomb);
	sprite().set_scale(1.5, 1.5);
	indicator().set_tex_and_coords(tex.item, coords.item_bomb_ind);
}
void BombItem::on_pick(Player* player) 
{
	player->extend_bomb();
}

void PointSmallItem::on_create() 
{
	SmallItem::on_create();
	set_start_info(-3.5, 60, false, false);
	sprite().set_tex_and_coords(tex.item, coords.item_point_small);
	sprite().set_alpha(0.5);
	sprite().set_blend(BlendMode::plus);
	if (auto p = GameSystem::instance()->get_player())
		pick(p);
}
void PointSmallItem::on_pick(Player* player)
{
	SmallItem::on_pick(player);
	player->add_point_score(1);
}
