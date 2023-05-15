#ifndef GALAGA_FORTMATION_JSONREADER
#define GALAGA_FORTMATION_JSONREADER

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "rapidjson/document.h"

class FormationJSONReader final
{
public:
	FormationJSONReader(const std::string& jsonPath);
	~FormationJSONReader();

	std::vector<glm::vec3> GetPositions(const std::string& enemyType);

private:
	rapidjson::Document m_JsonDoc;
};

#endif
