#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "Application.h"

#include "DrawSystem.h"
#include "InputSystem.h"
#include "GameSystem.h"
#include "WindowSystem.h"
#include "ObjectSystem.h"
#include "ParticleSystem.h"
#include "InputListener.h"
#include "Settings.h"
#include "SoundSystem.h"

#include "PStage.h"


class App final : public Application, public InputListener
{
	bool paused = false;

	void on_key_press(Key key) override
	{
		if (key == Key::escape)
		{
			this->exit();
		}
		if (key == Key::enter)
		{
			if (!paused)
			{
				paused = true;
				GameSystem::instance()->pause();
				SoundSystem::instance()->pause_all_bgm();
			}
			else
			{
				paused = false;
				GameSystem::instance()->resume();
				SoundSystem::instance()->resume_all_bgm();
			}
		}
	}

	void on_start() override
	{
		WindowSystem::instance();
		SoundSystem::instance();
		DrawSystem::instance();
		GameSystem::instance();

		WindowSystem::instance()->open
		(
			"simple stg 2 by NeuraXmy (under dev)", 
			settings::win_w,
			settings::win_h
		);
		WindowSystem::instance()->set_vsync(settings::vsync);

		SoundSystem::instance()->set_bgm_vol(settings::bgm_vol);
		SoundSystem::instance()->set_se_vol(settings::se_vol);

		DrawSystem::instance()->set_rand_seed(time(0));
		DrawSystem::instance()->adjust_to_current_window();

		GameSystem::instance()->set_rand_seed(time(0));
		GameSystem::instance()->start();

		GameSystem::instance()->append_stage<PStage>();
		GameSystem::instance()->start_from_stage(0);
	}

	double fps = 0.0;

	void on_update() override
	{
		WindowSystem::instance()->update();
		if (WindowSystem::instance()->is_closed())
		{
			exit();
		}

		SoundSystem::instance()->update();
		InputSystem::instance()->update();
		GameSystem::instance()->update();

		{
			DrawSystem::instance()->begin_frame();

			GameSystem::instance()->draw();

			DrawSystem::instance()->set_view_to_window();
			{
				if (get_frame_count() % 30 == 0)
					fps = get_frame_rate();

				std::ostringstream out;
				out << std::fixed << std::setprecision(3);
				out << "  O: " << ObjectSystem::instance()->get_object_count()
					<< "  P: " << ParticleSystem::instance()->get_count()
					<< "  FPS: " << fps;

				TextDrawcall t;
				t.str = out.str();
				t.size = 10;
				t.anchor = TextAnchor::right_buttom;
				t.x = GameSystem::ui_w - 20;
				t.y = GameSystem::ui_h - 5;
				t.color = Color(1, 1, 1);
				DrawSystem::instance()->draw_text(t);
			}
			DrawSystem::instance()->flush();


			DrawSystem::instance()->end_frame();
		}
	}

	void on_exit() override
	{
		GameSystem::release_instance();
		DrawSystem::release_instance();
		SoundSystem::release_instance();
		WindowSystem::release_instance();
	}

public:
	App() : Application(settings::vsync ? 0 : 60, 10, 120) {}
};

int main()
{
	std::fstream in("config.txt");
	if (in)
	{
		double scale = 1.0;
		in >> scale;
		settings::win_h *= scale;
		settings::win_w *= scale;
		in >> settings::vsync;
		in >> settings::bgm_vol;
		in >> settings::se_vol;
		in.close();
	}

	App app;
	app.start();
	return 0;
}