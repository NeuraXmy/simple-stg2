#pragma once

#include "Item.h"


class SmallItem : public Item
{
public:
	virtual	~SmallItem() = default;
protected:
	virtual void on_create() override = 0;
};

class BigItem : public Item
{
public:
	virtual	~BigItem() = default;
protected:
	virtual void on_create() override = 0;
};

class PowerItem final : public SmallItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};

class PointItem final : public SmallItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};

class PowerBigItem final : public BigItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};

class FullItem final : public BigItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};

class LifeItem final : public BigItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};

class BombItem final : public BigItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};

class PointSmallItem final : public SmallItem
{
	void on_create() override;
	void on_pick(Player* player) override;
};