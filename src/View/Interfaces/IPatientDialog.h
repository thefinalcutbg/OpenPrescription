#pragma once

class Patient;
class Validator;
enum class Insured;
enum PatientField { id, fname, mname, lname, city, phone };

#include "View/Interfaces/AbstractLineEdit.h"
#include "View/Interfaces/AbstractDateEdit.h"
class IPatientDialog
{

public:

	virtual void setTitle(const std::string& title) = 0;

	virtual void close() = 0;
	virtual void setLn4View(bool show) = 0;
	virtual void setEditMode(bool editMode) = 0;
	virtual void setPatient(const Patient& patient) = 0;
	virtual void resetFields() = 0;
	virtual Patient getPatient() = 0;

	virtual AbstractLineEdit* lineEdit(PatientField field) = 0;
	virtual AbstractDateEdit* dateEdit() = 0;
};

