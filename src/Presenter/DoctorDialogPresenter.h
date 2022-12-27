#pragma once
#include "Model/Doctor.h"
#include "Model/Validators/NameValidator.h"
#include "Model/Validators/CommonValidators.h"
#include "Model/Validators/PatientValidators.h"
#include "Model/Validators/LpkValidator.h"
#include "Model/Validators/RziValidaor.h"
#include "View/Interfaces/IDoctorSettingsDialog.h"

class DoctorDialogPresenter
{
	bool m_editMode;
	bool m_newDoctor;

	IDoctorSettingsDialog* view{nullptr};

	std::string current_LPK;

	std::optional<Doctor> result;

	EgnValidator egn_validator;
	NameValidator name_validator;
	LpkValidator lpk_validator;
	NotEmptyValidator not_emptyValidator;
	RziValidator rzi_validator;

public:
	//DoctorDialogPresenter();
	DoctorDialogPresenter();
	DoctorDialogPresenter(const Doctor& doctor);

	void okPressed();

	//void validLPK(const std::string& validLPK);

	void setView(IDoctorSettingsDialog* view);

	std::optional<Doctor> open();
};

