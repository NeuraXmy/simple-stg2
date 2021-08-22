#include "InputListener.h"
#include "InputSystem.h"


InputListener::InputListener()
{
	InputSystem::instance()->reg_listener(this);
}

InputListener::~InputListener()
{
	InputSystem::instance()->remove_listener(this);
}


void InputListener::on_key_press(Key key)
{

}

void InputListener::on_key_release(Key key)
{

}

void InputListener::on_mouse_press(MouseButton button)
{

}

void InputListener::on_mouse_release(MouseButton button)
{

}

void InputListener::on_mouse_move(double x, double y)
{

}
