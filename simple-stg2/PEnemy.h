#pragma once

#include "PResource.h"
#include "Enemy.h"

class ESPrefab : public Enemy
{
	ESColor color;
	int drop_power = 0;
	int drop_point = 0;

public:

	ESPrefab(ESColor color, int drop_power = 0, int drop_point = 0) 
		: color(color), drop_power(drop_power), drop_point(drop_point) {}

	virtual ~ESPrefab() = default;

protected:

	void on_create() override;

	void on_drop_item() override;

};


class EBPrefab : public Enemy
{
	int drop_power = 0;
	int drop_point = 0;

public:

	EBPrefab(int drop_power = 0, int drop_point = 0)
		: drop_power(drop_power), drop_point(drop_point) {}

	virtual ~EBPrefab() = default;

protected:

	void on_create() override;

	void on_drop_item() override;

};