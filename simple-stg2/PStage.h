#pragma once
#include "Stage.h"
#include "PResource.h"

class PStage : public Stage
{
	void on_create() override;
	void on_update() override;
};