#pragma once

#include "GamePanel.h"
#include "Sprite.h"


class PPanel final : public GamePanel
{
	Sprite panel;
	Sprite line;
	Sprite no_bomb;
	Sprite bomb;
	Sprite no_life;
	Sprite life;
	Sprite boss_indicator;
public:
	PPanel();
private:
	void on_draw() override;
};