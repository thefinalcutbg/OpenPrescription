#pragma once

#include <vector>
#include "View/Interfaces/IMainView.h"
#include "TabPresenter.h"
#include "ListSelectorPresenter.h"
#include "Network/HIS/UpdateMedications.h"

class MainPresenter
{
	TabPresenter m_tabPresenter;
	ListSelectorPresenter m_listSelector;
	UpdateMedications num_update_service;
	IMainView* view{nullptr};

public:
	MainPresenter();

	void editDoctor();
	void setView(IMainView* view);
	void newPerscriptionPressed();
	void showPatients();
	void showPrescriptions();
	bool save();
	void userSettingsPressed();
	//returns false when user selects cancel, otherwise returns true
	bool closeAllTabs();
};

