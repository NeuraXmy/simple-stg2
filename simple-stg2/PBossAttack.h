#pragma once
#include "BossAttack.h"
#include "GameSystem.h"
#include "ObjectSystem.h"
#include "PItem.h"
#include "PBullet.h"
#include "Shooter.h"
#include "Boss.h"

class SpellCard : public BossAttack
{
protected:
	virtual void on_create() override;
public:
	virtual ~SpellCard() = default;
};

class NormalAttack : public BossAttack
{
protected:
	virtual void on_create() override;
public:
	virtual ~NormalAttack() = default;
};
