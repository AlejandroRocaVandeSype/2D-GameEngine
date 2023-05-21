#include "FormationReaderCP.h"
#include "GameObject.h"
#include "rapidjson/rapidjson.h"		// To read json files
#include "rapidjson/istreamwrapper.h"
#include <fstream>						// for both input and output files


FormationReaderCP::FormationReaderCP(engine::GameObject* pOwner)
	: Component("FormationReaderCP", pOwner)
	, m_FormationJsonDoc{}
{
}

void FormationReaderCP::OpenJSONFile(const std::string& jsonFilePath)
{
	rapidjson::Document formationJsonDoc;

	// Open the json file to read
	std::ifstream ifs(jsonFilePath, std::ios::in);
	if (!ifs.is_open())
	{
		throw std::runtime_error("Error: Failed to open file: " + jsonFilePath);
	}

	// Try to parse the json file 
	rapidjson::IStreamWrapper isw{ ifs };
	formationJsonDoc.ParseStream(isw);             // Store in the document all the information
	if (formationJsonDoc.HasParseError())
	{
		throw std::runtime_error("Error: Could not parse the " + jsonFilePath + "file.");
	}
	
	ifs.close();

	m_FormationJsonDoc = std::move((formationJsonDoc));
	m_IsLoaded = true;
}


std::vector<std::pair<std::string, glm::vec3>> FormationReaderCP::ReadFormation(const std::string& jsonFilePath, const std::string& enemyType)
{
	if (!m_IsLoaded)
	{
		// Try to open the jsonFile in order to read it
		OpenJSONFile(jsonFilePath);
	}
	
	std::vector<std::pair<std::string, glm::vec3>> enemyInfo;

	const rapidjson::Value& element = m_FormationJsonDoc;
	if (m_FormationJsonDoc.HasMember("units"))
	{
		const rapidjson::Value& units = element["units"].GetArray();
		for (rapidjson::SizeType i{ 0 }; i < units.Size(); ++i)
		{
			const rapidjson::Value& unit = units[i];
			std::string unitType = unit["type"].GetString();
			if (unitType == enemyType)
			{
				// Is the one we want info about
				std::string startPos = unit["startPosition"].GetString();		// Enemies will apear from top, left .....
				const rapidjson::Value& positions = unit["formationPos"].GetArray();
				for (rapidjson::SizeType pos{ 0 }; pos < positions.Size(); ++pos)
				{
					const rapidjson::Value& value = positions[pos].GetArray();					
					enemyInfo.emplace_back(std::make_pair(startPos, glm::vec3{ value[0].GetFloat(), value[1].GetFloat(), 0 }));

				}

			}
		}
	}

	return enemyInfo;
}

std::vector<std::pair<std::string, unsigned short>> FormationReaderCP::ReadOrder(const std::string& jsonFilePath)
{
	if (!m_IsLoaded)
	{
		// Try to open the jsonFile in order to read it
		OpenJSONFile(jsonFilePath);
	}

	// It will contain the order in which enemies will spawn and the amount per batch
	std::vector<std::pair<std::string, unsigned short>> orderInfo;

	const rapidjson::Value& element = m_FormationJsonDoc;
	if (m_FormationJsonDoc.HasMember("orderInfo"))
	{
		const rapidjson::Value& orders = element["orderInfo"].GetArray();
		for (rapidjson::SizeType i{ 0 }; i < orders.Size(); ++i)
		{
			const rapidjson::Value& order = orders[i];
			std::string type = order["type"].GetString();		// From top, left, right ....
			unsigned short amount = static_cast<unsigned short>(order["amount"].GetInt());
			
			orderInfo.emplace_back(std::make_pair(type, amount));

		}
	}


	return orderInfo;
}

void FormationReaderCP::ClearJSONFile()
{
	m_IsLoaded = false;
}

void FormationReaderCP::Update(const float)
{

}
void FormationReaderCP::ReceiveMessage(const std::string&, const std::string&)
{

}

FormationReaderCP::~FormationReaderCP()
{

}