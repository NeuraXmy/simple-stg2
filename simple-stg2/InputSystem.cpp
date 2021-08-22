#include "InputSystem.h"
#include "InputListener.h"
#include "WindowSystem.h"

namespace
{
	InputSystem* instance = nullptr;

	sf::Keyboard::Key key_mapping[(int)(Key::key_num)] =
	{
		sf::Keyboard::Key::Z,
		sf::Keyboard::Key::X,
		sf::Keyboard::Key::C,
		sf::Keyboard::Key::Up,
		sf::Keyboard::Key::Down,
		sf::Keyboard::Key::Left,
		sf::Keyboard::Key::Right,
		sf::Keyboard::Key::LShift,
		sf::Keyboard::Key::D,
		sf::Keyboard::Key::Enter,
		sf::Keyboard::Key::Escape
	};

	sf::Mouse::Button mouse_button_mapping[(int)(MouseButton::mouse_button_num)] =
	{
		sf::Mouse::Button::Left,
		sf::Mouse::Button::Right
	};
}


InputSystem* InputSystem::instance()
{
	if (!::instance)
	{
		::instance = new InputSystem;
	}
	return ::instance;
}

void InputSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}

InputSystem::InputSystem() : 
	blocked(false),
	key_on(),
	mouse_button_on()
{

}

InputSystem::~InputSystem()
{

}


void InputSystem::reg_listener(InputListener* listener)
{
	listeners.push_back(listener);
}

void InputSystem::remove_listener(InputListener* listener)
{
	listeners.remove(listener);
}

void InputSystem::block_input(bool block)
{
	blocked = block;
}

void InputSystem::update()
{
	if (blocked)
	{
		return;
	}

	sf::RenderWindow* window = WindowSystem::instance()->get_window();
	sf::Event event;
	while (window && window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}

	for (int key = 0; key < (int)(Key::key_num); key++)
	{
		sf::Keyboard::Key sf_key = key_mapping[key];

		if (sf::Keyboard::isKeyPressed(sf_key))
		{
			if (!key_on[sf_key])
			{
				key_on[sf_key] = true;
				for (auto l : listeners)
				{
					l->on_key_press(static_cast<Key>(key));
				}
			}
		}
		else
		{
			if (key_on[sf_key])
			{
				key_on[sf_key] = false;
				for (auto l : listeners)
				{
					l->on_key_release(static_cast<Key>(key));
				}
			}
		}
	}

	for (auto l : listeners)
	{
		l->on_mouse_move(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
	}

	for (int button = 0; button < (int)(MouseButton::mouse_button_num); button++)
	{
		sf::Mouse::Button sf_btn = mouse_button_mapping[button];

		if (sf::Mouse::isButtonPressed(sf_btn))
		{
			if (!mouse_button_on[sf_btn])
			{
				mouse_button_on[sf_btn] = true;
				for (auto l : listeners)
				{
					l->on_mouse_press(static_cast<MouseButton>(button));
				}
			}
		}
		else
		{
			if (mouse_button_on[sf_btn])
			{
				mouse_button_on[sf_btn] = false;
				for (auto l : listeners)
				{
					l->on_mouse_release(static_cast<MouseButton>(button));
				}
			}
		}
	}

}
