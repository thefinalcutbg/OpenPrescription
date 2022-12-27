#include "PatientInfoPresenter.h"
#include "Presenter/PatientDialogPresenter.h"

#include "Database/DbPatient.h"
#include "View/ModalDialogBuilder.h"


PatientInfoPresenter::PatientInfoPresenter(IPatientTileInfo* view, std::shared_ptr<Patient> p) :
    view(view), patient(p)
{}

void PatientInfoPresenter::setDate(const Date& date)
{
    current_date = date;

    view->setPresenter(this);
    view->setPatient(*patient, patient->getAge(current_date));
}

void PatientInfoPresenter::setCurrent()
{
    view->setPresenter(this);
    view->setPatient(*patient, patient->getAge(current_date));
}

void PatientInfoPresenter::patientTileClicked()
{
    if (patient == nullptr) return;

    PatientDialogPresenter p{ *patient };

    auto patient = p.open();

    if (!patient.has_value()) return;

    *this->patient = patient.value();

   // tabPresenter->refreshPatientTabNames(patient->rowid);

    view->setPatient(*patient, patient->getAge(current_date));
}
