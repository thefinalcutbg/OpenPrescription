#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QPainter>

#include "Presenter/PatientDialogPresenter.h"
#include "ui_PatientFormDialog.h"
#include "View/Interfaces/IPatientDialog.h"


#include "Model/Patient.h"

class PatientFormDialog : public QDialog, public IPatientDialog
{

    QRegularExpressionValidator* phoneValidator;
    QRegularExpressionValidator* numValidator;
    QRegularExpressionValidator* nameValidator;


    Ui::PatientFormDialog ui;

    void paintEvent(QPaintEvent* event) override;

    PatientDialogPresenter* presenter;

    std::array<LineEdit*, 6> patientFields;

public:
    Q_OBJECT


public:

    PatientFormDialog(PatientDialogPresenter* p, QWidget* parent = 0);
    ~PatientFormDialog();

    void setEditMode(bool editMode) override;
    void close() override;
    void setLn4View(bool show) override;
    void setTitle(const std::string& title) override;
    void resetFields() override;

    void setPatient(const Patient& patient) override;
    Patient getPatient() override;
    AbstractLineEdit* lineEdit(PatientField field) override;
    AbstractDateEdit* dateEdit() override;
};
