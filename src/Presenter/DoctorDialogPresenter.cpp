#include "DoctorDialogPresenter.h"
#include "View/Interfaces/AbstractLineEdit.h"
#include "Database/DbDoctor.h"
#include "View/ModalDialogBuilder.h"

DoctorDialogPresenter::DoctorDialogPresenter()
{
}

DoctorDialogPresenter::DoctorDialogPresenter(const Doctor& doctor) :
    result(doctor)
{}

void DoctorDialogPresenter::okPressed()
{
    AbstractLineEdit* lineCheck[6]
    {
        view->lineEdit(DoctorFields::LPK),
        view->lineEdit(DoctorFields::RZI),
        view->lineEdit(DoctorFields::EGN),
        view->lineEdit(DoctorFields::FirstName),
        view->lineEdit(DoctorFields::LastName),
        view->lineEdit(DoctorFields::Phone),
    };

    for (auto& line : lineCheck)
    {
        line->validateInput();

        if (!line->isValid())
        {
            line->setFocus();
            return;
        }
    }

    auto doctor = view->getDoctor();

    if (!doctor.specialty.isValid()) {
        ModalDialogBuilder::showMessage("Моля, изберете специалност!");
        return;
    }

    DbDoctor::hasDoctor() ? DbDoctor::update(doctor)
        : DbDoctor::insertDoctor(doctor);

    result.emplace(doctor);

    view->close();
}

void DoctorDialogPresenter::setView(IDoctorSettingsDialog* view)
{
    this->view = view;

    view->lineEdit(DoctorFields::EGN)->setInputValidator(&egn_validator);
    view->lineEdit(DoctorFields::FirstName)->setInputValidator(&name_validator);
    view->lineEdit(DoctorFields::LastName)->setInputValidator(&name_validator);
    view->lineEdit(DoctorFields::LPK)->setInputValidator(&lpk_validator);
    view->lineEdit(DoctorFields::Phone)->setInputValidator(&not_emptyValidator);
    view->lineEdit(DoctorFields::RZI)->setInputValidator(&rzi_validator);

    if (result.has_value()) {
        view->setDoctor(result.value());
        result.reset();
    }
        
}

std::optional<Doctor> DoctorDialogPresenter::open()
{
    ModalDialogBuilder::openDialog(this);
    
    return result;
}
