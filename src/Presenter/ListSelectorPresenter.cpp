﻿#include "ListSelectorPresenter.h"
#include "View/Interfaces/IListSelectorView.h"
#include "View/ModalDialogBuilder.h"
#include "Database/DbPatient.h"

ListSelectorPresenter::ListSelectorPresenter()
{}

void ListSelectorPresenter::openDialog(TabType type)
{
	m_currentModelType = type;

	if (!view) {
		m_selectedInstances.clear();
		ModalDialogBuilder::openDialog(this);
	}
	else view->focus();


}

void ListSelectorPresenter::setView(IListSelectorView* view)
{
	this->view = view;

	if (!view) return;

	view->setDates(m_from, m_to);

	refreshModel();
	
}

void ListSelectorPresenter::setDates(const Date& from, const Date& to)
{
	m_from = from;
	m_to = to;
	refreshModel();
}



void ListSelectorPresenter::refreshModel()
{
	
	m_patientRows = DbListOpener::getPatientRows();
	m_perscriptionRows = DbListOpener::getPrescriptionRows(m_from, m_to);
	
	if (view != nullptr)
	{
		setListType(m_currentModelType);
		view->setDates(m_from, m_to);
	}
		
}

void ListSelectorPresenter::setListType(TabType type)
{
	m_currentModelType = type;
	m_selectedInstances.clear();

	switch (type)
	{
		case::TabType::PatientSummary: view->setRows(m_patientRows); break;
		case::TabType::Prescription: view->setRows(m_perscriptionRows); break;
	}
}

void ListSelectorPresenter::selectionChanged(std::set<int> selectedIndexes)
{ 

	m_selectedInstances.clear();

	switch (m_currentModelType) {
		case::TabType::PatientSummary: for (auto idx : selectedIndexes) m_selectedInstances.push_back(&m_patientRows[idx]); break;
		case::TabType::Prescription: for (auto idx : selectedIndexes) m_selectedInstances.push_back(&m_perscriptionRows[idx]); break;
	}
}

#include "Presenter/TabPresenter.h"

void ListSelectorPresenter::setTabPresenter(TabPresenter* tabPresenter)
{
	this->tab_presenter = tabPresenter;
}



void ListSelectorPresenter::openNewDocument(TabType type)
{
	for (int i = 0; i < m_selectedInstances.size(); i ++) {

		RowInstance row(type);
		row.rowID = 0;
		row.patientRowId = m_selectedInstances[i]->patientRowId;

		tab_presenter->open(row, i == m_selectedInstances.size() - 1);
	}

	view->close();
}


void ListSelectorPresenter::openCurrentSelection()
{
	if (!m_selectedInstances.size()) return;

	auto size = m_selectedInstances.size();

	int counter{ 0 };

	for (auto& row : m_selectedInstances) {

		bool isLastTab = ++counter == size;

		tab_presenter->open(*row, isLastTab);
	}

	if (view) view->close();
}




void ListSelectorPresenter::deleteCurrentSelection()
{
	if (m_selectedInstances.empty()) return;

	std::string warningMsg = "Сигурни ли сте, че искате да изтриете избраният/избраните ";

	static constexpr const char* endString[5]{
		"пацинети? Всичките свързани с тях рецепти ще бъдат изтрити!",
		"рецепти?"
	};

	warningMsg += endString[static_cast<int>(m_currentModelType)];

	if (!ModalDialogBuilder::askDialog(warningMsg))
		return;

	for (auto& row : m_selectedInstances)
	{
		if (tab_presenter->documentTabOpened(row->type, row->rowID))
		{
			ModalDialogBuilder::showMessage
			("Първо затворете всички избрани за изтриване документи!");
			return;
		}
	}

	for (auto& row : m_selectedInstances) {
		DbListOpener::deleteRecord(row->type, row->rowID);
	}


	refreshModel();
	
}

ListSelectorPresenter::~ListSelectorPresenter()
{
}

