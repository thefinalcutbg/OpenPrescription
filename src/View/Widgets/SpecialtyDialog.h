#pragma once

#include <QDialog>
#include <QSortFilterProxyModel>
#include "ui_SpecialtyDialog.h"
#include "View/Models/SpecialtyTableModel.h"

class SpecialtyDialog : public QDialog
{
	Q_OBJECT

	int m_selectedRow{ -1 };

	QSortFilterProxyModel m_proxyModel;
	SpecialtyTableModel m_model;

public:
	SpecialtyDialog(const QString& name = QString(), QWidget* parent = nullptr);
	
	std::string getResult();
	~SpecialtyDialog();

private:
	Ui::SpecialtyDialogClass ui;
};
