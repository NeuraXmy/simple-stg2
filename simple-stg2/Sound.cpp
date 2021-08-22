#include "Sound.h"
#include "SoundSystem.h"


namespace Sound
{

	Se::~Se()
	{
		stop();
	}

	double Se::get_vol() const
	{
		return vol;
	}

	void Se::set_vol(double vol)
	{
		this->vol = vol;
		update_vol();
	}

	double Se::get_real_vol() const
	{
		return vol * ::SoundSystem::instance()->get_se_vol();
	}

	void Se::play()
	{
		if(!is_playing() || get_time() >= play_interval)
			sound.play();
	}

	void Se::pause()
	{
		sound.pause();
	}

	void Se::stop()
	{
		sound.stop();
	}

	void Se::set_time(int time)
	{
		sound.setPlayingOffset(sf::seconds(time / 60.0));
	}

	int Se::get_time() const
	{
		return sound.getPlayingOffset().asSeconds() * 60;
	}

	bool Se::is_playing() const
	{
		return sound.getStatus() == sf::Sound::Playing;
	}

	bool Se::is_paused() const
	{
		return sound.getStatus() == sf::Sound::Paused;
	}

	int Se::get_play_interval() const
	{
		return play_interval;
	}

	void Se::set_play_interval(int interval)
	{
		play_interval = interval;
	}

	void Se::update_vol()
	{
		sound.setVolume(get_real_vol() * 100.0);
	}



	Bgm::~Bgm()
	{
		stop();
	}

	double Bgm::get_vol() const
	{
		return vol;
	}

	void Bgm::set_vol(double vol)
	{
		this->vol = vol;
		update_vol();
	}

	double Bgm::get_real_vol() const
	{
		return vol * ::SoundSystem::instance()->get_bgm_vol();
	}

	std::wstring Bgm::get_title() const
	{
		return title;
	}

	void Bgm::set_title(const std::wstring& title)
	{
		this->title = title;
	}

	void Bgm::play()
	{
		sound.play();
	}

	void Bgm::pause()
	{
		sound.pause();
	}

	void Bgm::stop()
	{
		sound.stop();
	}

	void Bgm::set_time(int time)
	{
		sound.setPlayingOffset(sf::seconds(time / 60.0));
	}

	int Bgm::get_time() const
	{
		return sound.getPlayingOffset().asSeconds() * 60;
	}

	bool Bgm::is_playing() const
	{
		return sound.getStatus() == sf::Sound::Playing;
	}

	bool Bgm::is_paused() const
	{
		return sound.getStatus() == sf::Sound::Paused;
	}

	void Bgm::update()
	{
		if (is_playing())
		{
			for (auto [time, to] : controls)
			{
				if (get_time() >= time)
				{
					if (to >= 0)
						set_time(to);
					else
						stop();
					break;
				}
			}
		}
	}

	void Bgm::update_vol()
	{
		sound.setVolume(get_real_vol() * 100.0);
	}
}