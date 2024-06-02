#pragma once
#include <string>
#include <vector>
#include <optional>
#include "Specialty.h"
#include "Model/Date.h"

constexpr const char* doctorPrefix = "д-р ";

struct Doctor
{
	int rowID;
	std::string RZI;
	std::string LPK;
	std::string fname;
	std::string lname;
	Specialty specialty;
	std::string phone;

	std::string getFullName(bool prefix = true) const
	{
		return prefix ?
			doctorPrefix + fname + " " + lname
			:
			fname + " " + lname;
	}

	std::string RHIF() const {
		
		auto firstTwoDigits = std::stoi(RZI.substr(0, 2));

		if (firstTwoDigits > 28) {
			firstTwoDigits -= 30;
		}

		return std::to_string(firstTwoDigits);
	}

};


