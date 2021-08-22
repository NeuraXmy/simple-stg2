#pragma once

#include <string>
#include "Sound.h"


class SoundSystem final
{
public:

	static SoundSystem* instance();

	static void release_instance();

	
	Sound::Se* load_se(const std::string& sound_path);

	void clear_all_se();

	double get_se_vol() const;

	void set_se_vol(double vol);

	void stop_all_se();

	void pause_all_se();

	void resume_all_se();


	Sound::Bgm* load_bgm(const std::string& sound_path, const std::string& info_path);

	void clear_all_bgm();

	double get_bgm_vol() const;

	void set_bgm_vol(double vol);

	void stop_all_bgm();

	void pause_all_bgm();

	void resume_all_bgm();


	void update();


private:

	SoundSystem();

	~SoundSystem();


	std::vector<Sound::Se*> se;
	double se_vol = 1.0;

	std::vector<Sound::Bgm*> bgm;
	double bgm_vol = 1.0;

};

