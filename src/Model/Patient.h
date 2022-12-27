#pragma once
#include <string>
#include "Date.h"
#include <memory>
#include <array>
#include <optional>
#include <vector>
#include "Ekatte.h"


struct Patient
{
	long long rowid{ 0 };

	int type{ 1 };
	std::string id;
	Date birth;

	bool sex{ 0 };

	std::string FirstName;
	std::string MiddleName;
	std::string LastName;
	Ekatte city;
	std::string phone;

	static bool getSexFromEgn(const std::string& egn);
	int getAge(const Date& currentDate = Date::currentDate())  const;
	bool isAdult(const Date& currentDate = Date::currentDate())  const;
	Date turns18At() const;
	std::string fullName() const;
	std::string firstLastName() const;
	std::string getFullAddress() const;

	~Patient();
};
