#include "TabPresenter.h"
#include "Model/Patient.h"
#include "Model/TableRows.h"
#include "Database/DbPatient.h"
#include "PrescriptionPresenter.h"

#include <TinyXML/tinyxml.h>

TabPresenter::TabPresenter() : _indexCounter(-1), m_currentIndex(-1), view(nullptr)
{}

void TabPresenter::setView(ITabView* view)
{
    this->view = view;
    view->setTabPresenter(this);
}

TabInstance* TabPresenter::currentTab()
{
    if (!m_tabs.count(m_currentIndex))
    {
        return nullptr;
    }

    return m_tabs[m_currentIndex];
}


void TabPresenter::createNewTab(TabInstance* tabInstance, bool setFocus)
{
    _indexCounter++;

    m_tabs[_indexCounter] = tabInstance;

    tabInstance->setContainerIdx(_indexCounter);

    view->newTab(_indexCounter, m_tabs[_indexCounter]->getTabName());

    //tabs size() > 1, because the first tab gets focused by the view automatically
    if (setFocus && m_tabs.size() > 1)
        view->focusTab(_indexCounter);
}

void TabPresenter::setCurrentTab(int index)
{
    if (currentTab() != nullptr) currentTab()->prepareSwitch();

    m_currentIndex = index;

    if (index == -1)
    {
        //it's 100% empty, but just in case...
        if (m_tabs.empty()) {
            _indexCounter = -1;
        }

        view->showDinosaur();
        return;
    }

    m_tabs[index]->setCurrent();
    
}

void TabPresenter::closeTabRequested(int tabId)
{
    if(permissionToClose(tabId)) removeTabInstance(tabId);
}

void TabPresenter::removeTabInstance(int tabId)
{
    delete m_tabs[tabId];
    m_tabs.erase(tabId);
    view->removeTab(tabId);
}

bool TabPresenter::permissionToLogOut()
{
    for (auto& pair : m_tabs)
    {
        if (!permissionToClose(pair.first))
            return false;
    }

    m_tabs.clear();
    view->removeAllTabs();

    return true;
}


std::shared_ptr<Patient> TabPresenter::getPatient_ptr(const Patient& patient)
{
    for (auto& [index, tabInstance] : m_tabs)
    {
        if (tabInstance->patient != nullptr &&
            tabInstance->patient->rowid == patient.rowid
            )
           // *tabInstance->patient.get() = patient;
            return tabInstance->patient;
    }

    auto result = std::make_shared<Patient>(patient);

    return result;
}


void TabPresenter::refreshPatientTabNames(long long patientRowId)
{
    for (auto tab : m_tabs) {

        if (tab.second->patient == nullptr ||
            tab.second->patient->rowid != patientRowId) {
            continue;
        }

        view->changeTabName(tab.second->getTabName(), tab.first);
    }
}


void TabPresenter::openPerscription(const Patient& patient)
{
    createNewTab(new PrescriptionPresenter(view, this, getPatient_ptr(patient)));
}


void TabPresenter::open(const RowInstance& row, bool setFocus)
{
    if (tabAlreadyOpened(row)) return;

    TabInstance* newTab{nullptr};

    switch (row.type)
    { 
        case TabType::Prescription:
        newTab = new PrescriptionPresenter(view, this, getPatient_ptr(DbPatient::get(row.patientRowId)), row.rowID);
        break;
        case TabType::PatientSummary:
        newTab = new PrescriptionPresenter(view, this, getPatient_ptr(DbPatient::get(row.patientRowId)), row.rowID);
        break;
    }

    createNewTab(newTab, setFocus);
}

bool TabPresenter::tabAlreadyOpened(const RowInstance& row)
{

    for (auto& [index, tab] : m_tabs)
    {
        if (tab->type == row.type && 
            tab->rowID() == row.rowID &&
            (tab->patient == nullptr || //financial tab edge case
            tab->patient->rowid == row.patientRowId )
        )
        {
            view->focusTab(index);
            return true;
        }
    }

    return false;
}



bool TabPresenter::permissionToClose(int tabId)
{
    return m_tabs[tabId]->close();
}

bool TabPresenter::documentTabOpened(TabType type, long long rowID) const
{
    for (const auto& [index, tab] : m_tabs)
    {

        if (tab->type == type && tab->rowID() == rowID)
        {
            return true;
        }
    }

    //if user wants to delete the patient, check other types of documents related to the patient
    if (type == TabType::PatientSummary)
    {
        for (const auto& [index, tab] : m_tabs)
        {       
            if (tab->patient.get()->rowid == rowID)
            {
                return true;
            }
        }
    }

    return false;
}
