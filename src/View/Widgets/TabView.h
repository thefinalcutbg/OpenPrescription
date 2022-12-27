#pragma once

#include <QWidget>

#include "Presenter/TabPresenter.h"
#include "View/Widgets/PrescriptionView.h"
#include "View/uiComponents/PixmapLabel.h"

#include "View/Interfaces/ITabView.h"
#include "ui_TabView.h"

class TabTitle;

class TabView : public QWidget, public ITabView
{
	Q_OBJECT

	TabPresenter* tabPresenter;
	
	PrescriptionView m_perscriptionView;

	PixmapLabel* noTabs;

	void showTabWidget(QWidget* w);

	TabTitle* getTabTitle(int tabId);
	int getTabIndex(int tabId);


public:
	TabView(QWidget *parent = Q_NULLPTR);
	~TabView();

	void requestClose(int tabId);

	// Inherited via ITabView
	void removeAllTabs();
	void newTab(int tabId, const TabName& tabName) override;
	void focusTab(int tabId) override;
	void removeCurrentTab() override;
	//void changeTabName(const TabName& tabName) override;
	void changeTabName(const TabName& tabName, int tabId) override;
	void setTabPresenter(TabPresenter* presenter) override;
	void removeTab(int tabId) override;

	ScrollPos getScrollPos();
	void setScrollPos(ScrollPos scrollPos);

	void showPerscriptionView() override;
	void showDinosaur() override;

	IPrescriptionView* perscriptionView() override { return &m_perscriptionView; }

signals:
	void closeRequested(int mapIndex);

private:
	Ui::TabView ui;




};
