#include "FormationReaderCP.h"
#include "GameObject.h"
#include "../../rapidjson/rapidjson.h"		// To read json files
#include "../../rapidjson/istreamwrapper.h"
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

// This reads correctly if the user respected the order of spawning.
// Spawning order is always top - left - right - top - top
std::vector<FormationReaderCP::EnemySpawnInfo*> FormationReaderCP::ReadSpawnOrder(const std::string& jsonFilePath)
{
	if (!m_IsLoaded)
	{
		// Try to open the jsonFile in order to read it
		OpenJSONFile(jsonFilePath);
	}
	
	std::vector<FormationReaderCP::EnemySpawnInfo*> vSpawnsInfo;

	// Keep track how many enemies from each type are gonna spawn
	// This way next bacth will take into account this amount to spawn the correct enemies
	short beesSpawnCount{};
	short butterfliesSpawnCount{};
	short galagasSpawnCount{};
	short currentCount{};

	const rapidjson::Value& element = m_FormationJsonDoc;
	if (m_FormationJsonDoc.HasMember("spawnInfo"))
	{
		const rapidjson::Value& spawns = element["spawnInfo"].GetArray();
		for (rapidjson::SizeType spawnsIdx{ 0 }; spawnsIdx < spawns.Size(); ++spawnsIdx)
		{
			const rapidjson::Value& spawn = spawns[spawnsIdx];

			std::string startPosition{ spawn["startPosition"].GetString() }; // From top, left, right ....
			const rapidjson::Value& enemyTypes = spawn["enemyTypes"].GetArray();
			for (rapidjson::SizeType enemyIdx{ 0 }; enemyIdx < enemyTypes.Size(); ++enemyIdx)
			{
				const rapidjson::Value& enemy = enemyTypes[enemyIdx];

				// Get all enemy types that will spawn at this position and the amount of them			
				std::string type{ enemy["type"].GetString() };
				short amount{ static_cast<short>( enemy["amount"].GetInt() ) };
				
				if (type == "bees")
				{
					beesSpawnCount += amount;
					currentCount = beesSpawnCount;
				}
				else if (type == "butterflies")
				{
					butterfliesSpawnCount += amount;
					currentCount = butterfliesSpawnCount;
				}
				else if (type == "galagas")
				{
					galagasSpawnCount += amount;
					currentCount = galagasSpawnCount;
				}

				EnemySpawnInfo* enemyInfo{ new EnemySpawnInfo(startPosition, type, currentCount) };
				vSpawnsInfo.emplace_back(enemyInfo);

			}
		}
	}

	return vSpawnsInfo;
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