#pragma once

#include "../../../Engine/Interfaces/IPaginatedMenu.h"
#include "SFMenu.h"

class SFPaginatedMenu : public IPaginatedMenu
{
public:
	SFPaginatedMenu() = default;
	~SFPaginatedMenu() = default;

	SFMenu* AddMenu(SFMenu* menu);
	SFMenu* GetMenuByNumber(int menuNumer);
	SFMenu* GetCurrentMenu() { return GetMenuByNumber(m_currentMenuNum); }
};