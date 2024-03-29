#pragma once
#include "Model/Doctor.h"

class DoctorDialogPresenter;
class AbstractLineEdit;


namespace DoctorFields
{
	enum Field {RZI, LPK, FirstName, LastName, Phone};
}


class IDoctorSettingsDialog
{
public:
	virtual void setDoctor(const Doctor& doctor) = 0;
	virtual Doctor getDoctor() = 0;
	virtual AbstractLineEdit* lineEdit(DoctorFields::Field field) = 0;
	virtual void close() = 0;
};