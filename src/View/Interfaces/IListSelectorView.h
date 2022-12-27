#pragma once

#include <vector>

#include "Model/TableRows.h"

class IListSelectorView
{
public:
	virtual void setDates(const Date& from, const Date& to) = 0;
	virtual void setRows(const std::vector<PatientRow>& rows) = 0;
	virtual void setRows(const std::vector<PrescriptionRow>& rows) = 0;
	virtual void focus() = 0;
	virtual void close() = 0;
};