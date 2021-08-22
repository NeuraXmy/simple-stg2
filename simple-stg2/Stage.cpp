#include "Stage.h"
#include "Background.h"


void Stage::start()
{
	if (!started)
	{
		started = true;
		ended = false;
		resume_update();
		on_start();
	}
}

bool Stage::is_started() const
{
	return started;
}

void Stage::end()
{
	if (!ended)
	{
		ended = true;
		started = false;
		pause_update();
		on_end();
	}
}

bool Stage::is_ended() const
{
	return ended;
}

Background* Stage::background() const
{
	return bg;
}

void Stage::remove_background()
{
	if (bg)
	{
		delete bg;
		bg = nullptr;
	}
}

void Stage::on_create()
{
	GameObject::on_create();

	pause_update();
}

void Stage::on_update()
{
	GameObject::on_update();

	if (bg)
	{
		bg->update(t());
	}
}

void Stage::on_destroy()
{
	remove_background();

	GameObject::on_destroy();
}

void Stage::on_draw()
{
	GameObject::on_draw();

	if (bg)
	{
		bg->draw(t());
	}
}


void Stage::on_start()
{
	
}

void Stage::on_end()
{

}