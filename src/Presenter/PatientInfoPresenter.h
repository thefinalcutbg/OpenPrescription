#pragma once
#include <memory>

#include "Model/Patient.h"

#include "View/Interfaces/IPatientTileInfo.h"

class PatientInfoPresenter
{
	std::shared_ptr<Patient> patient;
	IPatientTileInfo* view;

	Date current_date{ Date::currentDate() };

	bool m_current{ false };

public:
	PatientInfoPresenter(IPatientTileInfo* view, std::shared_ptr<Patient> p);
	void setDate(const Date& date);

	void patientTileClicked();
	void setCurrent();


};