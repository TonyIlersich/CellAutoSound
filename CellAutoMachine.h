#pragma once

#include "includes.h"

class CellAutoMachine : public sf::Drawable
{
private:

	sf::Sprite sprite;
	sf::RenderTexture rt[2];
	bool current = 0;

public:

	sf::Shader shader;

	CellAutoMachine(unsigned int width, unsigned int height, sf::Image initial = sf::Image());
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	const sf::Texture& getTexture() const;
};

