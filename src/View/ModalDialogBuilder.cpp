#include "ModalDialogBuilder.h"

#include "View/Widgets/PatientFormDialog.h"

void ModalDialogBuilder::openDialog(PatientDialogPresenter* p)
{
	PatientFormDialog d(p);
	d.exec();
}


#include "View/Widgets/SelectorDialog.h"

void ModalDialogBuilder::openDialog(ListSelectorPresenter* p)
{
	SelectorDialog d(p);

	d.exec();
}

#include "View/Widgets/MedicationDialog.h"

void ModalDialogBuilder::openDialog(MedicationPresenter* p)
{
	MedicationDialog d(p);
	d.exec();
}

#include "View/Widgets/DosageDialog.h"

void ModalDialogBuilder::openDialog(DosagePresenter* p)
{
	DosageDialog d(p);
	d.exec();
}

#include "View/Widgets/DoctorSettingsDialog.h"

void ModalDialogBuilder::openDialog(DoctorDialogPresenter* p)
{
	DoctorSettingsDialog d(p);
	d.exec();
}

#include "View/Widgets/SaveDialog.h"

DialogAnswer ModalDialogBuilder::openSaveDialog(const std::string& title)
{
	SaveDialog s(QString::fromStdString(title));
	int answer = s.exec();

	switch (answer)
	{
	case QMessageBox::Yes:
		return DialogAnswer::Yes;
	case QMessageBox::No:
		return DialogAnswer::No;
	default:
		return DialogAnswer::Cancel;
	}
}

#include <QFileDialog>
/*
std::optional<std::string> ModalDialogBuilder::getMonthNotifFromFile()
{
	QString filePath = QFileDialog::getOpenFileName(nullptr, 
		"Изберете месечно известие", "", "XML files(*.xml)");

	if (filePath.isEmpty())
		return {};
	return filePath.toStdString();
}
*/

bool ModalDialogBuilder::askDialog(const std::string& questionText)
{
	QMessageBox msg;

	msg.setWindowTitle("OpenPrescription");

	msg.setText(QString::fromStdString(questionText));
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
	msg.setButtonText(QMessageBox::Yes, "Да");
	msg.setButtonText(QMessageBox::No, "Не");
	msg.setIcon(QMessageBox::Question);

	return msg.exec() == QMessageBox::Yes;
}

void ModalDialogBuilder::showError(const std::string& error)
{

	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle("Възникна грешка");
	msgBox.setText(QString::fromStdString(error));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();

}

void ModalDialogBuilder::showMessage(const std::string& message)
{
	QMessageBox msgBox;
	msgBox.setModal(true);
	msgBox.setText(QString::fromStdString(message));
	msgBox.exec();
}


#include <QInputDialog>


#include "View/Widgets/PinPromptDialog.h"

std::string ModalDialogBuilder::pinPromptDialog(const std::string& pem)
{
	PinPromptDialog d(pem);
	d.exec();

	return d.getResult().toStdString();
}

#include "View/Widgets/InputDialog.h"

std::string ModalDialogBuilder::inputDialog(const std::string& text, const std::string& title, bool asPassword)
{
	InputDialog d(asPassword);
	d.setLabel(text);
	d.setTitle(title);
	d.exec();
	return d.result();
}



