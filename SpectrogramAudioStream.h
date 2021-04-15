#pragma once

#include "includes.h"

class SpectrogramAudioStream : public sf::SoundStream
{
	const int sampleRate = 44100;
	const double baseFreq = 110; // A2
	const size_t samplesPerChunk;
	std::vector<sf::Int16> samples;
	//sf::Mutex mutex;

public:

	SpectrogramAudioStream(size_t samplesPerChunk);
	bool onGetData(Chunk& data) override;
	void onSeek(sf::Time timeOffset) override;
	void parse(const sf::Texture& sgTex, double t0);

private:

	double getFreq(int delta) const;
};

