#pragma once
#include "GetNumenclature.h"

class UpdateMedications : public GetNumenclature
{
	 bool parseNumenclature(const std::string& reply) override;

public:
	bool show_conformation {false};
	void update();
};

