#pragma once

#include <list>


class InputListener;

enum class Key;
enum class MouseButton;


// 输入系统 该系统由WindowSystem管理
class InputSystem final
{
public:

	static InputSystem* instance();

	static void release_instance();


	void update();

	void reg_listener(InputListener* listener);

	void remove_listener(InputListener* listener);

	void block_input(bool block);

private:

	InputSystem();

	~InputSystem();


	std::list<InputListener*> listeners;

	bool blocked;

	bool key_on[256];

	bool mouse_button_on[16];

};

