#include "PatientTileInfo.h"
#include "Presenter/PatientInfoPresenter.h"

PatientTileInfo::PatientTileInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.patientTile, &QPushButton::clicked, [=] {
		if (presenter) presenter->patientTileClicked();
	});

}

void PatientTileInfo::setPatient(const Patient& p, int age)
{
	ui.patientTile->setData(p, age);
}

PatientTileInfo::~PatientTileInfo()
{}
