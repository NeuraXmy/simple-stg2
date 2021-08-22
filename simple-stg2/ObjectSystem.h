#pragma once

#include <list>

class GameObject;


// 该系统由GameSystem进行管理，包括初始化、释放、更新、绘制
class ObjectSystem final
{
public:

	static ObjectSystem* instance();

	static void release_instance();


	void update();


	void draw();

	void draw_ui();


	template<class Object, class ... Args>
	Object* create_object(Args ... args)
	{
		Object* obj = new Object(args...);
		obj->create();
		objects.push_back(obj);
		return obj;
	}

	void clear_objects();

	int get_object_count() const;


	template<class Object, class Func>
	void foreach(const Func& func)
	{
		for (auto obj : objects)
			if (auto p = dynamic_cast<Object*>(obj))
				func(p);
	}

private:

	std::list<GameObject*> objects;

	ObjectSystem();

	~ObjectSystem();

};
