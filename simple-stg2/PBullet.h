#pragma once

#include "PResource.h"
#include "EnemyBullet.h"

extern const BBColor bs_to_bb_color[];
extern const BSColor bb_to_bs_color[];
extern const Color bs_color[];
extern const Color bb_color[];

class BSPrefab : public EnemyBullet
{
	BSType type;
	BSColor color;
	bool create_effect;

public:

	BSPrefab(BSType type, BSColor color, bool create_effect = false) 
		: type(type), color(color), create_effect(create_effect) {};

	virtual ~BSPrefab() = default;

protected:

	virtual void on_create() override;
	virtual void on_drop_item() override;
};


class BBPrefab : public EnemyBullet
{
	BBType type;
	BBColor color;
	bool create_effect;

public:

	BBPrefab(BBType type, BBColor color, bool create_effect = false) 
		: type(type), color(color), create_effect(create_effect) {};

	virtual ~BBPrefab() = default;

protected:

	virtual void on_create() override;
	virtual void on_drop_item() override;

};