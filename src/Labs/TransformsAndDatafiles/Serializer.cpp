#include "Serializer.h"
/*Jserializer::Jserializer() {

}

bool Jserializer::serialize(JsonSerializer * newplanet, std::string serializedoutput) {
	if (newplanet == NULL)
		return false;

	Json::Value Sroot;
	newplanet->serialize(Sroot);

	Json::StyledWriter jwriter;
	serializedoutput = jwriter.write(Sroot);

	return true;
};

bool Jserializer::deserialize(JsonSerializer * newplanet, std::string deserialisedinput)
{
	if (newplanet == NULL)
	return false;

	Json::Value DSroot;
	Json::Reader jreader;

	if (!jreader.parse(deserialisedinput, DSroot)) {
		return false;
	}

	newplanet->deserialize(DSroot);

	return true;

}*/
