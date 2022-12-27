#pragma once
#include <vector>
#include "Model/TableRows.h"
#include "Model/Patient.h"

namespace DbListOpener
{
	std::vector<PatientRow> getPatientRows();
	std::vector<PrescriptionRow> getPrescriptionRows(const Date& from, const Date& to);
	void deleteRecord(TabType type, long long rowid);
};

