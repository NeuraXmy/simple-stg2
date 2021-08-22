#pragma once

enum class Key
{
	shoot,
	bomb,
	special,
	up,
	down,
	left,
	right,
	slow,
	debug,
	enter,
	escape,
	key_num
};

enum class MouseButton
{
	left,
	right,
	mouse_button_num
};


class InputListener
{
public:

	InputListener();

	virtual ~InputListener();


	virtual void on_key_press(Key key);

	virtual void on_key_release(Key key);

	virtual void on_mouse_press(MouseButton button);

	virtual void on_mouse_release(MouseButton button);

	virtual void on_mouse_move(double x, double y);

};

