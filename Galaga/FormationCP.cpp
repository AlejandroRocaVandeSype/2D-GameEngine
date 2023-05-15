#include "FormationCP.h"
#include "GameObject.h"
#include "FormationJSONReader.h"
#include <memory>
#include <iostream>

FormationCP::FormationCP(engine::GameObject* pOwner, [[maybe_unused]] const std::string& formationJsonPath)
	:Component("FormationCP", pOwner)
{

	

	ReadFormationFromJSON(formationJsonPath);

}

void FormationCP::ReadFormationFromJSON(const std::string& formationJsonPath)
{
	std::unique_ptr<FormationJSONReader> jsonReader = std::make_unique<FormationJSONReader>(formationJsonPath);

	m_vBeesPos.clear();
	m_vButterfliesPos.clear();
	m_vGalagasPos.clear();

	m_vBeesPos = jsonReader->GetPositions("bees");
	m_vButterfliesPos = jsonReader->GetPositions("butterflies");
	m_vGalagasPos = jsonReader->GetPositions("galagas");

}

std::vector<glm::vec3> FormationCP::GetFormation(const std::string& enemyType) const
{
	if (enemyType == "bees")
	{
		return m_vBeesPos;
	}

	if(enemyType == "butterflies")
	{
		return m_vButterfliesPos;
	}

	if (enemyType == "galagas")
	{
		return m_vGalagasPos;
	}

	std::vector<glm::vec3> emptyVector;
	return emptyVector;
}

FormationCP::~FormationCP()
{

}

void FormationCP::Update([[maybe_unused]] const float deltaTime)
{

}

void FormationCP::ReceiveMessage([[maybe_unused]] const std::string& message, [[maybe_unused]] const std::string& value)
{

}