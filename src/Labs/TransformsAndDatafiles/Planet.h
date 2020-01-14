#pragma once
#include "Serializer.h"
#include <glm/glm.hpp>
class Planet : public JsonSerializer
{
public:
	Planet();
	virtual ~Planet();
	virtual void Jserialize(Json::Value& root);
	virtual void Jdeserialize(Json::Value& root);
private:
	std::string name;
	std::string object;
	
	int positionx;
	int positiony;
	int positionz;
	int position = (positionx, positiony, positionz);
	//glm::vec3 position;
};