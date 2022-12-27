#pragma once
#include <string>
#include <variant>
#include "Model/Date.h"
#include "Presenter/TabInstance.h"

struct RowInstance
{
	const TabType type;
	long long rowID{0};
	long long patientRowId{0};

	RowInstance(TabType t) : type(t) {}
};

struct PatientRow : public RowInstance
{
	PatientRow() : RowInstance{ TabType::PatientSummary } {};

	std::string patientId;
	std::string name;
	std::string phone;
};

struct PrescriptionRow : public RowInstance
{
	PrescriptionRow() : RowInstance{ TabType::Prescription } {};

	Date date;
	std::string nrn;
	std::string patientId;
	std::string patientName;
	std::string patientPhone;
};
