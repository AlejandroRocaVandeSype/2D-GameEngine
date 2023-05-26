#ifndef GALAGA_FORTMATION_READER
#define GALAGA_FORTMATION_READER

#include <Component.h>
#include <string>
#include <glm/vec3.hpp>
#include "rapidjson/document.h"
#include <vector>

// Class that contains two json documents with information about the Formation of enemies from the current stage
class FormationReaderCP final : public engine::Component
{
public:

	struct EnemySpawnInfo
	{
	public:
		EnemySpawnInfo(std::string startPos, std::string type, short indexMaxCount)
			: startPosition{ startPos }, enemyType{ type }, spawnIdxMaxCount{ indexMaxCount } {}

		const std::string& GetStartPos() { return startPosition; }
		const std::string& GetEnemyType() { return enemyType; }
		short GetSpawnAmount() { return spawnIdxMaxCount; }

	private:
		const std::string startPosition;
		const std::string enemyType;
		const short spawnIdxMaxCount;		// Indicates until which index we can active enemies from this type
											// It takes into account the amount of enemies from the same type that
											// have spawned already
	};

	FormationReaderCP(engine::GameObject* pOwner);
	~FormationReaderCP();

	void Update(const float) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

	void OpenJSONFile(const std::string& jsonFilePath);
	void ClearJSONFile();

	std::vector<std::pair<std::string, glm::vec3>> ReadFormation(const std::string& jsonFilePath, const std::string& enemyType);
	std::vector<FormationReaderCP::EnemySpawnInfo*> ReadSpawnOrder(const std::string& jsonFilePath);

private:
	rapidjson::Document m_FormationJsonDoc;		
	bool m_IsLoaded;
	
};

#endif

