#pragma once
#include <json\json\json\json.h>
class JsonSerializer {
public:
	virtual void serialize(Json::Value& root) = 0;
	virtual void deserialize(Json::Value& root) = 0;
};

class Jserializer {
public:
	Jserializer();
	static bool serialize(JsonSerializer* newplanet, std::string output);
	static bool deserialize(JsonSerializer* newplanet, std::string output);

private:
};