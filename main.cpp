#include "includes.h"
#include "shaders.h"
#include "CellAutoMachine.h"
#include "SpectrogramAudioStream.h"
#include "RenderBuffer.h"
using namespace std;

sf::Mutex globalMutex;

constexpr int frameWidth = 16;
constexpr int frameHeight = 48;
constexpr double frameDuration = 2;

void drawGlider(sf::Image& img, int x, int y)
{
	img.setPixel(x + 1, y + 0, sf::Color::White);
	img.setPixel(x + 2, y + 1, sf::Color::White);
	img.setPixel(x + 0, y + 2, sf::Color::White);
	img.setPixel(x + 1, y + 2, sf::Color::White);
	img.setPixel(x + 2, y + 2, sf::Color::White);
}

void drawHLine(sf::Image& img, int x, int y, int w)
{
	int end = x + w;
	for (; x < end; x++)
		img.setPixel(x, y, sf::Color::White);
}

void scramble(sf::Image& img)
{
	for (int x = 0; x < img.getSize().x; x++)
	{
		for (int y = 0; y < img.getSize().y; y++)
		{
			sf::Uint8 v = (rand() % 2 == 0) * 255;
			img.setPixel(x, y, sf::Color(v, v, v));
		}
	}
}

void drawRandomMelody(sf::Image& img)
{
	for (int x = 0; x < img.getSize().x; x++)
	{
		int y = rand() % img.getSize().y;
		img.setPixel(x, y, sf::Color::White);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Cell Auto Soundscape");
	window.setView(sf::View(sf::FloatRect(0, 0, frameWidth, frameHeight)));

	sf::Image initialImage;
	initialImage.create(frameWidth, frameHeight);
	//scramble(initialImage);
	drawRandomMelody(initialImage);
	CellAutoMachine dbrt(frameWidth, frameHeight, initialImage);

	RenderBuffer<1> rb(frameWidth, frameHeight);
	rb.setSource(dbrt);

	double simulatedTime = 0;

	SpectrogramAudioStream sas(44100 * frameDuration);
	sas.parse(dbrt.getTexture(), simulatedTime);
	simulatedTime += frameDuration;

	dbrt.shader.loadFromMemory(melodyCode, sf::Shader::Fragment);

	sf::Clock clock;
	sas.play();
	clock.restart();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				sas.stop();
				break;
			}
		}

		globalMutex.lock();

		cerr << "draw" << endl;

		window.draw(rb);
		window.display();

		cerr << "update" << endl;

		dbrt.update();

		cerr << "parse" << endl;

		sas.parse(dbrt.getTexture(), simulatedTime);
		simulatedTime += frameDuration;

		globalMutex.unlock();

		while (window.isOpen() && clock.getElapsedTime().asSeconds() + frameDuration < simulatedTime);

		cerr << "loop done" << endl << endl;
	}

	return 0;
}