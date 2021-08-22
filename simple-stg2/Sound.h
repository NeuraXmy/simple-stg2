#pragma once

#include <SFML/Audio.hpp>

class SoundSystem;


namespace Sound
{
	class Se final
	{
		friend class ::SoundSystem;

	public:

		~Se();

		double get_vol() const;

		void set_vol(double vol);

		double get_real_vol() const;


		void play();

		void pause();

		void stop();

		void set_time(int time);

		int get_time() const;


		bool is_playing() const;

		bool is_paused() const;


		int get_play_interval() const;

		void set_play_interval(int interval);

	private:

		double vol = 1.0;

		sf::SoundBuffer buffer;
		sf::Sound sound;

		int play_interval = 0;

		void update_vol();

	};


	class Bgm final
	{
		friend class ::SoundSystem;

	public:

		~Bgm();


		double get_vol() const;

		void set_vol(double vol);

		double get_real_vol() const;

		std::wstring get_title() const;

		void set_title(const std::wstring& title);


		void play();

		void pause();

		void stop();

		void set_time(int time);

		int get_time() const;


		bool is_playing() const;

		bool is_paused() const;
	

		void update();

	private:

		std::wstring title;

		double vol = 1.0;

		sf::SoundBuffer buffer;
		sf::Sound sound;

		std::vector<std::pair<int, int>> controls;


		void update_vol();

	};
};

