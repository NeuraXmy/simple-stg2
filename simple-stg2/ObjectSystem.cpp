#include "ObjectSystem.h"
#include "GameObject.h"


namespace
{
	ObjectSystem* instance = nullptr;
}

ObjectSystem* ObjectSystem::instance()
{
	if (!::instance)
	{
		::instance = new ObjectSystem;
	}
	return ::instance;
}

void ObjectSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}

ObjectSystem::ObjectSystem()
{

}

ObjectSystem::~ObjectSystem()
{
	clear_objects();
}


void ObjectSystem::update()
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		auto obj = *it;
		obj->update();
	}

	for (auto it = objects.begin(); it != objects.end(); )
	{
		auto obj = *it;
		if (obj->to_be_destroy())
		{
			delete obj;
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ObjectSystem::draw()
{
	for (auto obj : objects)
	{
		obj->draw();
	}
}

void ObjectSystem::draw_ui()
{
	for (auto obj : objects)
	{
		obj->draw_ui();
	}
}


void ObjectSystem::clear_objects()
{
	for (auto obj : objects)
	{
		obj->destroy();
	}
	for (auto obj : objects)
	{
		delete obj;
	}
	objects.clear();
}

int ObjectSystem::get_object_count() const
{
	return objects.size();
}
