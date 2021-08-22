#pragma once

#include "Player.h"


class Reimu final : public Player
{
	void on_create() override;
	void on_update() override;
	void on_graze(EnemyBullet* b) override;
	void on_graze(Laser* l) override;
	void on_kill() override;
};