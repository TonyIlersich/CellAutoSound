#include "SpectrogramAudioStream.h"
using namespace std;

SpectrogramAudioStream::SpectrogramAudioStream(size_t samplesPerChunk):
	samplesPerChunk(samplesPerChunk), samples(samplesPerChunk)
{
	initialize(1, 44100);
}

bool SpectrogramAudioStream::onGetData(Chunk& data)
{
	cerr << "=========[ chunk ]=========" << endl;
	globalMutex.lock();
	data.samples = &samples.front();
	data.sampleCount = samplesPerChunk;
	globalMutex.unlock();
	return true;
}

void SpectrogramAudioStream::onSeek(sf::Time timeOffset)
{
	/* This function does nothing. */
}

void SpectrogramAudioStream::parse(const sf::Texture& sgTex, double t0)
{
	const sf::Image sgImg = sgTex.copyToImage();
	const sf::Vector2u size = sgImg.getSize();
	sf::Clock stopwatch;
	for (int i = 0; (size_t)i < samplesPerChunk; i++)
	{
		const double time = t0 + (double)i / sampleRate;
		double sample = 0;
		for (int j = 0; (unsigned)j < size.y; j++)
		{
			const int x = i * size.x / samplesPerChunk;
			const double strength = sgImg.getPixel(x, j).r / 255.0;
			sample += strength * sin(tao * getFreq(j) * time);
		}
		samples[i] = (sf::Int16)(sample * 32767 / size.y);
	}
}

double SpectrogramAudioStream::getFreq(int delta) const
{
	return baseFreq * pow(2, delta / 12.0);
}
