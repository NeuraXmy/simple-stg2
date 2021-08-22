#pragma once

#include "Stage.h"
#include "GameSystem.h"
#include "DrawSystem.h"
#include "Bullet.h"
#include "PhysicObject.h"
#include "SpriteObject.h"


class TestStage1 : public Stage
{
	Texture::Texture* bullet_tex;
	Texture::Coords* bullet_coord;

	void on_start() override
	{
		bullet_tex = DrawSystem::instance()->load_texture("res/tex/bullet/bullet_preset.png");
		bullet_coord = DrawSystem::instance()->load_texcoords("res/tex/bullet/seed.txc");
	}

	void on_update() override
	{
		auto gs = GameSystem::instance();

		for (int i = 0; i < 3; i++)
		{
			
		}
	}

	void on_end() override
	{

	}

};

