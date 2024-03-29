﻿#include "PatientDialogPresenter.h"
#include "View/ModalDialogBuilder.h"
#include "Database/DbPatient.h"

PatientDialogPresenter::PatientDialogPresenter() :
	view(nullptr)
{}

PatientDialogPresenter::PatientDialogPresenter(const Patient& patient) :
	m_patient(patient),
	rowid(patient.rowid),
	view(nullptr)
{
	if (patient.type == 1) {
		m_patient->sex = Patient::getSexFromEgn(patient.id);
		m_patient->birth = Date::getBirthdateFromEgn(patient.id);
	}
}

std::optional<Patient> PatientDialogPresenter::open()
{
	ModalDialogBuilder::openDialog(this);
	return m_patient;
}

void PatientDialogPresenter::setView(IPatientDialog* view)
{
	this->view = view;

	birth_validator.setMaxDate(Date::currentDate());
	birth_validator.setMaxErrorMsg("Невалидна рождена дата");
	birth_validator.setMinDate(Date(2, 1, 1900));
	birth_validator.setMinErrorMsg("Невалидна рождена дата");

	view->lineEdit(city)->setInputValidator(&city_validator);
	view->lineEdit(fname)->setInputValidator(&name_validator);
	view->lineEdit(mname)->setInputValidator(&cyrillic_validator);
	view->lineEdit(lname)->setInputValidator(&name_validator);
	view->dateEdit()->setInputValidator(&birth_validator);
	changePatientType(1);

	if (m_patient.has_value())
	{
		setPatientToView(m_patient.value());
		view->setEditMode(true);
	}

}

void PatientDialogPresenter::changePatientType(int index)
{
	switch (index)
	{
	case 1:
		view->setLn4View(false);

		view->lineEdit(id)->setInputValidator(&egn_validator);
		view->lineEdit(id)->validateInput();
		view->resetFields();
		break;
	case 2:
		view->setLn4View(true);

		view->lineEdit(id)->setInputValidator(&ln4_validator);
		view->lineEdit(id)->validateInput();;
		view->resetFields();
		break;
	default:
		break;

	}
}


void PatientDialogPresenter::accept()
{

	if (!viewIsValid()) return;

	m_patient = getPatientFromView();
	
	bool success{ false };

	if (rowid == 0) {
		rowid = m_patient->rowid;
		m_patient->rowid = DbPatient::insert(m_patient.value());

		if (!m_patient->rowid) m_patient.reset();
	}
	else
	{
		if (!DbPatient::update(m_patient.value())) m_patient.reset();
	}

	if (!m_patient) {
		ModalDialogBuilder::showError("Неуспешно записване в базата данни");
	}
	
	view->close();
}

void PatientDialogPresenter::searchDbForPatient(int type)
{
	std::string patientId = view->lineEdit(id)->getText();

	Patient patient = DbPatient::get(patientId, type);

	if (patient.rowid)
	{
		rowid = patient.rowid;
	}
	
	setPatientToView(patient);
}


void PatientDialogPresenter::setPatientToView(const Patient& patient)
{
	changePatientType(patient.type);

	view->setPatient(patient);
	
	view->lineEdit(id)->validateInput();

}

bool PatientDialogPresenter::viewIsValid()
{
	auto inputIsValid = [](AbstractUIElement* uiElement)
	{
		uiElement->validateInput();
		if (!uiElement->isValid())
		{
			uiElement->setFocus();
			return false;
		}
		return true;
	};

	for (int i = 0; i < 6; i++)
	{
		if (!inputIsValid(view->lineEdit(static_cast<PatientField>(i)))) return false;
	}

	if (!inputIsValid(view->dateEdit())) return false;

	return true;
}

Patient PatientDialogPresenter::getPatientFromView()
{
	Patient patient = view->getPatient();

	patient.rowid = rowid;

	return patient;
}