#include "SoundSystem.h"
#include <fstream>

namespace
{
	SoundSystem* instance = nullptr;
}

SoundSystem* SoundSystem::instance()
{
	if (!::instance)
	{
		::instance = new SoundSystem;
	}
	return ::instance;
}

void SoundSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}


SoundSystem::SoundSystem()
{

}

SoundSystem::~SoundSystem()
{
	clear_all_se();
	clear_all_bgm();
}


Sound::Se* SoundSystem::load_se(const std::string& sound_path)
{
	Sound::Se* s = new Sound::Se;
	if (!s->buffer.loadFromFile(sound_path))
	{
		return nullptr;
	}
	s->sound.setBuffer(s->buffer);
	se.push_back(s);
	s->update_vol();
	return s;
}

void SoundSystem::clear_all_se()
{
	stop_all_se();
	for (auto s : se)
	{
		delete s;
	}
	se.clear();
}

double SoundSystem::get_se_vol() const
{
	return se_vol;
}

void SoundSystem::set_se_vol(double vol)
{
	se_vol = vol;
	for (auto s : se)
	{
		s->update_vol();
	}
}

void SoundSystem::stop_all_se()
{
	for (auto s : se)
	{
		s->stop();
	}
}

void SoundSystem::pause_all_se()
{
	for (auto s : se)
	{
		if (s->is_playing())
		{
			s->pause();
		}
	}
}

void SoundSystem::resume_all_se()
{
	for (auto s : se)
	{
		if (s->is_paused())
		{
			s->play();
		}
	}
}


Sound::Bgm* SoundSystem::load_bgm(const std::string& sound_path, const std::string& info_path)
{
	Sound::Bgm* b = new Sound::Bgm;
	if (!b->buffer.loadFromFile(sound_path))
	{
		return nullptr;
	}
	
	std::ifstream in(info_path);
	if (!in)
	{
		delete b;
		return nullptr;
	}
	
	/*
	wchar_t wch = 0;
	while (in.read((char*)(&wch), 2) && wch != L'\n')
		b->title += wch;
	*/
	
	int time, to;
	while (in >> time >> to)
	{
		b->controls.emplace_back(time, to);
	}

	in.close();

	b->sound.setBuffer(b->buffer);
	bgm.push_back(b);
	b->update_vol();
	return b;
}

void SoundSystem::clear_all_bgm()
{
	stop_all_bgm();
	for (auto b : bgm)
	{
		delete b;
	}
	bgm.clear();
}

double SoundSystem::get_bgm_vol() const
{
	return bgm_vol;
}

void SoundSystem::set_bgm_vol(double vol)
{
	bgm_vol = vol;
	for (auto b : bgm)
	{
		b->update_vol();
	}
}

void SoundSystem::stop_all_bgm()
{
	for (auto b : bgm)
	{
		b->stop();
	}
}

void SoundSystem::pause_all_bgm()
{
	for (auto b : bgm)
	{
		if (b->is_playing())
		{
			b->pause();
		}
	}
}

void SoundSystem::resume_all_bgm()
{
	for (auto b : bgm)
	{
		if (b->is_paused())
		{
			b->play();
		}
	}
}


void SoundSystem::update()
{
	for (auto b : bgm)
	{
		b->update();
	}
}
