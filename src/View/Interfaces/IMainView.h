#pragma once

#include <string>

class ITabView;

class IMainView
{
	
public:
	bool m_loggedIn{ false };
	virtual ITabView* tabView() = 0;
	virtual void setUserLabel(const std::string& doctorName) = 0;
	virtual void exitProgram() = 0;

};