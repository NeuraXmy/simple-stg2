#include "WindowSystem.h"
#include "InputSystem.h"


namespace
{
	WindowSystem* instance = nullptr;
}

WindowSystem* WindowSystem::instance()
{
	if (!::instance)
	{
		::instance = new WindowSystem();
	}
	return ::instance;
}

void WindowSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}

WindowSystem::WindowSystem() :
	window(nullptr)
{
	InputSystem::instance();
}

WindowSystem::~WindowSystem()
{
	if (is_open())
	{
		close();
	}
	InputSystem::release_instance();
}


void WindowSystem::open(const std::string& title, int w, int h, bool full_screen)
{
	if (is_open())
	{
		close();
	}

	if (full_screen)
	{
		window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], title, sf::Style::Fullscreen);
	}
	else
	{
		window = new sf::RenderWindow(sf::VideoMode(w, h), title);
		set_focus();
	}
}

void WindowSystem::close()
{
	if (is_open())
	{
		delete window;
		window = nullptr;
	}
}

void WindowSystem::resize(int w, int h)
{
	if (!is_open())
	{
		return;
	}
	window->setSize(sf::Vector2u(w, h));
}

void WindowSystem::set_title(const std::string& title)
{
	window->setTitle(title);
}

int WindowSystem::get_w() const
{
	return is_open() ? window->getSize().x : 0;
}

int WindowSystem::get_h() const
{
	return is_open() ? window->getSize().y : 0;
}

void WindowSystem::set_vsync(bool flag)
{
	window->setVerticalSyncEnabled(flag);
}

void WindowSystem::set_focus()
{
	window->requestFocus();
}


void WindowSystem::update()
{
	if (is_open())
	{
		if (!window->isOpen())
		{
			close();
		}
	}
}


sf::RenderWindow* WindowSystem::get_window()
{
	return window;
}

const sf::RenderWindow* WindowSystem::get_window() const
{
	return window;
}

bool WindowSystem::is_open() const
{
	return window;
}

bool WindowSystem::is_closed() const
{
	return !window;
}
