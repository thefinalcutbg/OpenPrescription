#include "DoctorSettingsDialog.h"
#include "Presenter/DoctorDialogPresenter.h"
#include <unordered_map>
#include <QPainter>
#include "Model/Specialty.h"
#include "SpecialtyDialog.h"
void DoctorSettingsDialog::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(rect(), QColor(Qt::white));
}

DoctorSettingsDialog::DoctorSettingsDialog(DoctorDialogPresenter* presenter, QWidget *parent)
	: QDialog(parent), presenter(presenter)
{
	ui.setupUi(this);

	setWindowTitle("Доктор");
	setWindowIcon(QIcon{ ":/icons/icon_user.png" });

	lineEdits[DoctorFields::RZI] = ui.rziEdit;
	lineEdits[DoctorFields::FirstName] = ui.fNameEdit;
	lineEdits[DoctorFields::LastName] = ui.lNameEdit;
	lineEdits[DoctorFields::Phone] = ui.phoneEdit;
	lineEdits[DoctorFields::LPK] = ui.lpkEdit;


	for (auto& lineEdit : lineEdits)
	{
		lineEdit->setErrorLabel(ui.errorLabel);
	}

	connect(ui.okButton, &QPushButton::clicked, [=] {presenter->okPressed();});

	connect(ui.specialtyButton, &QPushButton::clicked,
		[=] {
			SpecialtyDialog d(ui.specialtyButton->text());
			d.exec();

			auto result = d.getResult();

			if (d.getResult().size()) {
				ui.specialtyButton->setText(result.c_str());
				ui.specialtyButton->adjustSize();
			}
		}
	);

	presenter->setView(this);
}

void DoctorSettingsDialog::setDoctor(const Doctor& doctor)
{
	ui.rziEdit->set_Text(doctor.RZI);
	ui.lpkEdit->set_Text(doctor.LPK);
	ui.fNameEdit->set_Text(doctor.fname);
	ui.lNameEdit->set_Text(doctor.lname);
	ui.phoneEdit->set_Text(doctor.phone);

	if (doctor.specialty.isValid()) {
		ui.specialtyButton->setText(doctor.specialty.getName().c_str());
	}
	else
	{
		ui.specialtyButton->setText("Изберете");
	}

	ui.specialtyButton->adjustSize();

}

Doctor DoctorSettingsDialog::getDoctor()
{
	Doctor doctor;
	doctor.RZI = ui.rziEdit->getText();
	doctor.LPK = ui.lpkEdit->getText();
	doctor.fname = ui.fNameEdit->getText();
	doctor.lname = ui.lNameEdit->getText();
	doctor.phone = ui.phoneEdit->getText();
	doctor.specialty = ui.specialtyButton->text().toStdString();

	return doctor;
}

DoctorSettingsDialog::~DoctorSettingsDialog()
{
}
