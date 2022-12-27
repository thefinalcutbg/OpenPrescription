#pragma once
#include <string>

class TabPresenter;

class IPrescriptionView;

struct TabName;
struct ScrollPos { int height{ 0 }, width{ 0 }; };

class ITabView
{
public:
	virtual void newTab(int vecPos, const TabName& tabName) = 0;
	virtual void focusTab(int vecPos) = 0;
	virtual void removeCurrentTab() = 0;
	virtual void removeTab(int vecPos) = 0;
//	virtual void changeTabName(const TabName& tabName) = 0;
	virtual void changeTabName(const TabName& tabName, int presenterIdx) = 0;
	virtual void setTabPresenter(TabPresenter* presenter) = 0;
	virtual void removeAllTabs() = 0;

	virtual ScrollPos getScrollPos() = 0;
	virtual void setScrollPos(ScrollPos scrollPos) = 0;

	virtual void showPerscriptionView() = 0;
	virtual void showDinosaur() = 0;

	virtual IPrescriptionView* perscriptionView() = 0;

	
};