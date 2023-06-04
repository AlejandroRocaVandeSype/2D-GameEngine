#ifndef GALAGA_MENUSELECTIONCP
#define GALAGA_MENUSELECTIONCP

#include <Component.h>
#include <string>
#include <vector>
#include "glm/vec3.hpp"

class MenuSelectionCP final : public engine::Component
{
public:

	MenuSelectionCP(engine::GameObject* pOwner, const std::vector<std::pair<std::string, glm::vec3>>& menuOptions);
	virtual ~MenuSelectionCP() override;

	virtual void Update(const float deltaTime) override;
	virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

	void ChangeSelection(const int menuAction);
	
	std::string GetSelection();
	bool IsOptionSelected() const;
	void SetOptionSelected(bool isSelected);
	

private:

	const std::vector<std::pair<std::string, glm::vec3>> m_MenuOptions;
	size_t m_ActiveSelectionPos;
	float m_selectionXPos;
	bool m_IsSelected;

};

#endif