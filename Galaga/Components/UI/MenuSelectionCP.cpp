#include "MenuSelectionCP.h"
#include "GameObject.h"

MenuSelectionCP::MenuSelectionCP(engine::GameObject* pOwner, const std::vector<std::pair<std::string, glm::vec3>>& menuOptions)
	:Component("MenuSelectionCP", pOwner)
	, m_MenuOptions { menuOptions }
	, m_ActiveSelectionPos { }, m_IsSelected { false }, m_selectionXPos { }
{
	if (!m_MenuOptions.empty())
	{
		// At start no option is selected since it will be the Controls image
		m_ActiveSelectionPos = 0;
		m_selectionXPos = m_MenuOptions.at(0).second.x - 30.f;
		UpdateArrowPos();
	}

}

MenuSelectionCP::~MenuSelectionCP()
{

}

void MenuSelectionCP::Update(const float)
{

}

void MenuSelectionCP::ChangeSelection(const int menuAction)
{
	if (menuAction > 0)
	{
		// Next option
		if (m_ActiveSelectionPos == m_MenuOptions.size() - 1)
		{
			// Move to the first option
			m_ActiveSelectionPos = 0;
		}
		else
		{
			m_ActiveSelectionPos++;
		}
	}
	else if(menuAction < 0)
	{
		// Previous option
		if (m_ActiveSelectionPos <= 0)
		{
			m_ActiveSelectionPos = m_MenuOptions.size() - 1;
		}
		else
		{
			m_ActiveSelectionPos--;
		}		
	}

	UpdateArrowPos();

}

void MenuSelectionCP::UpdateArrowPos()
{
	// Change arrow pos
	auto arrowTransfCP{ GetOwner()->GetComponent<engine::TransformComponent>() };
	if (arrowTransfCP != nullptr)
	{
		auto newPos{ m_MenuOptions.at(m_ActiveSelectionPos).second };
		newPos.x = m_selectionXPos;
		arrowTransfCP->SetLocalPosition(newPos);
	}
}

bool MenuSelectionCP::IsOptionSelected() const
{
	return m_IsSelected;
}

std::string MenuSelectionCP::GetSelectionName() const
{
	if (m_ActiveSelectionPos >= 0 && m_ActiveSelectionPos < m_MenuOptions.size())
	{
		return m_MenuOptions.at(m_ActiveSelectionPos).first;
	}

	// Empty string to indicate an invalid selection
	return "";
}


void MenuSelectionCP::SetOptionSelected(bool isSelected)
{
	m_IsSelected = isSelected;
}

void MenuSelectionCP::ReceiveMessage(const std::string& , const std::string& )
{

}