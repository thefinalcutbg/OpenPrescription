#include <QtWidgets/QApplication>
#include "View/Widgets/OpenPrescription.h"
#include "Network/UpdateService/UpdateService.h"

bool initFunction();

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setFont(QFont("Segoe UI", 8));

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/icon_prescr.png"));

    if (UpdateService::restartForUpdate()) { return 0; };

    if (!initFunction()) { return 0; }

    OpenPrescription w;
    w.setWindowState(Qt::WindowMaximized);

    w.show();

    return a.exec();

}

#include "Database/Database.h"
#include "Model/User.h"
#include "Model/Prescription/Medication.h"
#include "Model/Prescription/DoseQuantity.h"
#include "Model/Prescription/WhenToTake.h"
#include "Model/Ekatte.h"
#include "Model/Specialty.h"
#include "Database/DbDoctor.h"
#include "Presenter/DoctorDialogPresenter.h"
#include "Path.h"
#include "DbUpdater.h"

bool initFunction() {

    Db::setFilePath(Path::getDbPath());

    if (!Db::createIfNotExist()) {

        ModalDialogBuilder::showError(
            "Неуспешно създаване на базата данни."
            "\nСтартирайте програмата в администраторски режим"
        );
        return false;
    };

    //Intializing static data
    Medication::initialize();
    Ekatte::initialize();
    DoseQuantity::initialize();
    Route::initialize();
    WhenToTake::initialize();
    Specialty::initialize();
    Db::showErrorDialog(true);
    DbUpdater::updateDb();

    if (!DbDoctor::hasDoctor())
    {
        ModalDialogBuilder::showMessage(
            "Стартирате програмата за първи път, моля добавете доктор"
        );

        if (!DoctorDialogPresenter().open()) return false;
    }

    return true;

}
