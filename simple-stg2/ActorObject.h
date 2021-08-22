#pragma once

#include <vector>

#include "SpriteObject.h"


class ActorObject : public SpriteObject
{
public:

	virtual ~ActorObject() = default;


	int current_act_state() const;

	void set_act_state(int state);

	int create_act_state(int frame_index);

	int get_act_frame_index(int state) const;

	void set_act_frame_index(int state, int index);

	bool is_vaild_act_state(int state) const;

	void clear_act_state();

	int last_change_act_state_time() const;


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;


	virtual void on_change_act_state(int state);

private:

	int current_state = 0;

	std::vector<int> state_mapping;

	int last_change_time = 0;

};

