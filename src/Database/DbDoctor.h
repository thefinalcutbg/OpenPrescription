#pragma once
#include "Model/User.h"

namespace DbDoctor
{
	Doctor getDoctor();
	bool hasDoctor();
	void update(const Doctor& doctor);
	void insertDoctor(const Doctor& doctor);
}