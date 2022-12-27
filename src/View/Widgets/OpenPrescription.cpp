#include "OpenPrescription.h"
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>
#include <QFontDatabase>
#include "View/Theme.h"

QColor blue(133, 207, 234);

OpenPrescription::OpenPrescription(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // showMaximized();
   // ui.perscrButton->setIcon(QIcon(":/icons/icon_prescr.png"));
   // ui.saveButton->setIcon(QIcon(":/icons/icon_save.png"));
   // ui.openButton->setIcon(QIcon(":/icons/icon_open.png"));

    ui.perscrButton->setPosition(HalfRoundedButton::Position::Alone);
    ui.openButton->setPosition(HalfRoundedButton::Position::Alone);
    ui.userButton->setPosition(HalfRoundedButton::Position::Alone);

   // connect(ui.saveButton, &QPushButton::clicked, [&] { presenter.save(); });
    connect(ui.openButton, &QPushButton::clicked, [&] {presenter.showListSelector(); });
    connect(ui.perscrButton, &QPushButton::clicked, [&] { presenter.newPerscriptionPressed(); });
    connect(ui.userButton, &QPushButton::clicked, [&] { presenter.editDoctor(); });

/*
    setStyleSheet(
        "QMenu{"
        " background-color: white;"
        " }"

        "   QMenu::item {"
        "   padding: 2px 20px 2px 20px;"
        "  border: 1px solid transparent; "// reserve space for selection border
        "   spacing: 20px;"
        "    }"


        "  QMenu::item:selected {"
        "       border-color: darkblue;"
        "        background: rgb(53, 120, 191, 150);"
        "       color:white;"
        "      }"

        "    QMenu::separator {"
        "       height: 2px;"
        "         margin: 2px 5px 2px 4px;"
        "      }"

        "     QMenu::indicator {"
        "       width: 20px;"
        "       height: 13px;"
        "    }"
    );
 */
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
