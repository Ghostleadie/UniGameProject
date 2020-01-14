#pragma once
#include <fstream>
#include <sstream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio\SoundStream.hpp>
class CustomStream : public sf::SoundStream
{
public:
	bool open(const std::string& location)
	{
		// Open the source and get audio settings
		
		unsigned int channelCount = ...;
		unsigned int sampleRate = ...;
		// Initialize the stream -- important!
		initialize(channelCount, sampleRate);
	}
private:
	virtual bool onGetData(Chunk& data)
	{
		// Fill the chunk with audio data from the stream source
		data.samples = ...;
		data.sampleCount = ...;
		// Return true to continue playing
		return true;
	}
	virtual void onSeek(Uint32 timeOffset)
	{
		// Change the current position in the stream source
	
	}
}
