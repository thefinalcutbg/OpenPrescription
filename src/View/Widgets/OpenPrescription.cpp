#include "OpenPrescription.h"
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>
#include <QFontDatabase>
#include "View/Theme.h"
#include "AboutDialog.h"
QColor blue(133, 207, 234);

OpenPrescription::OpenPrescription(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.aboutButton->setIcon(QIcon(":/icons/icon_question.png"));
    ui.medicationUpdate->setIcon(QIcon(":/icons/icon_refresh.png"));
    
    ui.prescrButton->setPosition(HalfRoundedButton::Position::Alone);
    ui.patientButton->setPosition(HalfRoundedButton::Position::Alone);
    ui.newButton->setPosition(HalfRoundedButton::Position::Alone);
    ui.userButton->setPosition(HalfRoundedButton::Position::Alone);
  //  ui.medicationUpdate->setPosition(HalfRoundedButton::Position::Alone);

    connect(ui.patientButton, &QPushButton::clicked, [&] {presenter.showPatients(); });
    connect(ui.prescrButton, &QPushButton::clicked, [&] {presenter.showPrescriptions(); });
    connect(ui.newButton, &QPushButton::clicked, [&] { presenter.newPerscriptionPressed(); });
    connect(ui.userButton, &QPushButton::clicked, [&] { presenter.editDoctor(); });
    connect(ui.aboutButton, &QPushButton::clicked, [&] { AboutDialog d; d.exec(); });
    connect(ui.medicationUpdate, &QPushButton::clicked, [&] { presenter.updateMedications(); });
    
    presenter.setView(this);
}

ITabView* OpenPrescription::tabView()
{
    return ui.tabView;
}

void OpenPrescription::setUserLabel(const std::string& doctorName)
{
    ui.userButton->setText(QString::fromStdString("  " + doctorName));
    ui.userButton->adjustSize();
}

void OpenPrescription::exitProgram()
{
    QApplication::quit();
}

bool OpenPrescription::initialized()
{
    return m_loggedIn;
}

void OpenPrescription::paintEvent(QPaintEvent* event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(rect(), Theme::mainBackgroundColor);
    painter.end();
}

void OpenPrescription::closeEvent(QCloseEvent* event)
{
    if (!presenter.closeAllTabs())
        event->ignore();

    foreach(QWidget * widget, QApplication::topLevelWidgets()) 
    {
        if (widget == this) continue;
        widget->close();
    }
}
