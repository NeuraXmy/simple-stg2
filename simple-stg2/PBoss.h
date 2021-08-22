#pragma once
#include "Boss.h"

class PBoss : public Boss
{
public:
	virtual ~PBoss() = default;
protected:
	virtual void on_create() override = 0;
};
