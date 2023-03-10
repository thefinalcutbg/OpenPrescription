#include "TabInstance.h"

void TabInstance::makeEdited()
{
	if (edited) return;

	edited = true;
	auto tabName = getTabName();
	tabName.header = "*" + tabName.header;
	_tabView->changeTabName(tabName, m_containerIdx);
}

void TabInstance::refreshTabName()
{
	if (m_containerIdx == -1) return;

	_tabView->changeTabName(getTabName(), m_containerIdx);
}

bool TabInstance::close()
{
	if (!isNew() && !edited) return true;

	DialogAnswer answer = ModalDialogBuilder::openSaveDialog(getTabName().toString());

	switch (answer)
	{
	case DialogAnswer::Yes:
		//is save interrupted?
		return save() ? true : false;

	case DialogAnswer::No: return true;

	case DialogAnswer::Cancel: return false;
	}
}

void TabInstance::setCurrent()
{
	is_current = true;

	setDataToView();

	switch (type)
	{
	case TabType::Prescription:_tabView->showPerscriptionView(); break;
	}

	setScrollPosition();
}

void TabInstance::prepareSwitch()
{
	m_scrollPos = _tabView->getScrollPos();
	bool is_current = false;
}
