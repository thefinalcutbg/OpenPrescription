#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>

#include "Database/Database.h"
#include "Presenter/MainPresenter.h"
#include "ui_OpenPrescription.h"

#include "View/Interfaces/IMainView.h"


class OpenPrescription : public QMainWindow, public IMainView
{
    Q_OBJECT

    MainPresenter presenter;

    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent* event);

public:
    OpenPrescription(QWidget *parent = Q_NULLPTR);
    ITabView* tabView() override;
    void setUserLabel(const std::string& doctorName) override;
    void exitProgram() override;
    bool initialized();

private:
    Ui::OpenPrescriptionClass ui;
};
