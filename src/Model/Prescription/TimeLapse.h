#pragma once
#include <string>
#include <vector>

class TimeLapse {

	enum Unit { Day, Year, Hour, Minute, Month, Second, Week };
	int m_unit{ Unit::Day };

public:

	TimeLapse();

	TimeLapse(int value) : value(value) {};

	double value{ 1 };

	//returns list of duration names according to the value(singular/plural)
	std::vector<std::string> getUnitNamesList() const;

	std::string getUnitName() const;
	int getUnitIndex() const { return m_unit; }

	std::string getNHISKey() const;

	//returns true on success
	bool setUnit(int unitKey);


};