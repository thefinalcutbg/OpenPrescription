#pragma once

#include <unordered_map>
#include "View/Interfaces/ITabView.h"
#include "TabInstance.h"

class AmbList;
class Patient;
struct RowInstance;


class TabPresenter
{

	std::unordered_map<int, TabInstance*> m_tabs;

	int _indexCounter;
	int m_currentIndex;


	ITabView* view;
	
	bool permissionToClose(int tabId);
	bool tabAlreadyOpened(const RowInstance& tab);

	void createNewTab(TabInstance* tabInstance, bool setFocus = true);

	std::shared_ptr<Patient> getPatient_ptr(const Patient& patient);

	void removeTabInstance(int tabId);
public:
	
	TabPresenter();

	void setView(ITabView* view);

	TabInstance* currentTab();
	void setCurrentTab(int index);

	void refreshPatientTabNames(long long patientRowId);

	void open(const RowInstance& row, bool setFocus = false);
	void openPerscription(const Patient& patient);

	bool documentTabOpened(TabType type, long long rowID) const;

	void closeTabRequested(int tabId);


	//returns false if not all of the tabs are removed(a.k.a. user breaks the operation)
	bool permissionToLogOut();
};

