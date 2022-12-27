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
	std::string egn;
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
		return RZI.substr(0, 2);
	}

};


