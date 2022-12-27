#pragma once

#include <string>
#include <vector>
#include <optional>
#include "Model/Prescription/Dosage.h"

namespace Parser
{
	std::string write(const std::vector<Dosage>& dosage);
	std::vector<Dosage> parseDosage(const std::string& dosage);
};