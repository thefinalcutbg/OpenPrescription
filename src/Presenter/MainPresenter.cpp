#include "MainPresenter.h"
#include "Network/HIS/HisToken.h"

#include "Presenter/PatientDialogPresenter.h"
#include "Network/PKCS11.h"
#include "Model/User.h"
#include "Presenter/DoctorDialogPresenter.h"
#include "Database/DbDoctor.h"
#include "Database/DbUpdateStatus.h"
MainPresenter::MainPresenter()
{
    User::setCurrentDoctor(DbDoctor::getDoctor());

    auto uDate = DbUpdateStatus::lastUpdated(DynamicNum::Medication);

    if (uDate == Date::currentDate()) return;

    num_update_service.update();
}

void MainPresenter::editDoctor()
{
    DoctorDialogPresenter d(User::doctor());
    auto result = d.open();

    if (result) {
        User::setCurrentDoctor(*result);
        view->setUserLabel(result->getFullName());
    }
}

void MainPresenter::setView(IMainView* view)
{
    this->view = view;

    m_tabPresenter.setView(view->tabView());
    m_listSelector.setTabPresenter(&m_tabPresenter);

    view->setUserLabel(User::doctor().getFullName());

}


void MainPresenter::newPerscriptionPressed()
{
    PatientDialogPresenter p;

    auto patient = p.open();

    if (patient.has_value())
        m_tabPresenter.openPerscription(patient.value());
}

void MainPresenter::showPatients()
{
    m_listSelector.openDialog(TabType::PatientSummary);
}


void MainPresenter::showPrescriptions()
{
    m_listSelector.openDialog(TabType::Prescription);
}

void MainPresenter::updateMedications()
{
    num_update_service.show_conformation = true;
    num_update_service.update();
}

bool MainPresenter::save()
{
    if(m_tabPresenter.currentTab())
        return m_tabPresenter.currentTab()->save();

    return true;
}

void MainPresenter::userSettingsPressed()
{
    /*
    DoctorDialogPresenter p(User::doctor());
    auto doctor = p.open();

    if (doctor.has_value())
        User::setCurrentDoctor(doctor.value());

    */
}

bool MainPresenter::closeAllTabs()
{
    return m_tabPresenter.permissionToLogOut();
}

