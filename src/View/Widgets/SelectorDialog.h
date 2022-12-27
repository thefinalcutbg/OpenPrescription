#pragma once

#include <QDialog>
#include "ui_SelectorDialog.h"
#include "View/Interfaces/IListSelectorView.h"
#include "View/Models/PatientTableModel.h"
#include "View/Models/PrescriptionTableModel.h"
#include <QSortFilterProxyModel>

class ListSelectorPresenter;

class SelectorDialog : public QDialog, public IListSelectorView
{
	Q_OBJECT
private:

	ListSelectorPresenter* presenter;

	QSortFilterProxyModel idFilter;
	QSortFilterProxyModel nameFilter;
	QSortFilterProxyModel phoneFilter;

	PatientTableModel patient_model;
	PrescriptionTableModel perscription_model;


public:
	SelectorDialog(ListSelectorPresenter *p);

	void setDates(const Date& from, const Date& to) override;

	void setRows(const std::vector<PatientRow>& rows) override;
	void setRows(const std::vector<PrescriptionRow>& rows) override;
	~SelectorDialog();

private:
	Ui::SelectorDialog ui;

	//shows the additional buttons and changes selectionMode

	void focus() override;
	void close() override;



};
