#include "FormationJSONReader.h"
#include "rapidjson/rapidjson.h"		// To read json files
#include "rapidjson/istreamwrapper.h"
#include <fstream>						// for both input and output files

FormationJSONReader::FormationJSONReader(const std::string& jsonPath)
	: m_JsonDoc{}
{
	// Open the json file to read
	std::ifstream ifs(jsonPath, std::ios::in);
	if (!ifs.is_open())
	{
		throw std::runtime_error("Error: Failed to open file: " + jsonPath);
	}

	rapidjson::IStreamWrapper isw{ ifs };
	m_JsonDoc.ParseStream(isw);             // Store in the document all the information
	if (m_JsonDoc.HasParseError())
	{
		throw std::runtime_error("Error: Could not parse the " + jsonPath + "file.");	
	}

	ifs.close();
}

std::vector<glm::vec3> FormationJSONReader::GetPositions(const std::string& enemyType)
{
	std::vector<glm::vec3> enemyPositions;

	//if (m_JsonDoc.IsNull())
	
		// Single Object
		const rapidjson::Value& element = m_JsonDoc;
		if (m_JsonDoc.HasMember("enemies"))
		{
			const rapidjson::Value& enemies = element["enemies"].GetArray();
			for (rapidjson::SizeType i{ 0 }; i < enemies.Size(); ++i)
			{
				const rapidjson::Value& enemy = enemies[i];
				std::string type = enemy["type"].GetString();

				if (type == enemyType)
				{
					const rapidjson::Value& positions = enemy["positions"].GetArray();
					for (rapidjson::SizeType pos{ 0 }; pos < positions.Size(); ++pos)
					{
						const rapidjson::Value& value = positions[pos].GetArray();
						enemyPositions.push_back(glm::vec3{ value[0].GetFloat(), value[1].GetFloat(), 0 });

					}

					break;
				}

			}
		}
	
	return enemyPositions;

}

FormationJSONReader::~FormationJSONReader()
{

}