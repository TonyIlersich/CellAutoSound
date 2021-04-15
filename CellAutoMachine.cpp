#include "CellAutoMachine.h"
using namespace std;

CellAutoMachine::CellAutoMachine(
	unsigned int width, unsigned int height, sf::Image initial
) {
	// initialize render textures
	for (auto& tex : rt) tex.create(width, height);
	// load initial image into first render texture
	sf::Texture temp;
	temp.create(width, height);
	if (initial.getSize() == sf::Vector2u(width, height))
		temp.loadFromImage(initial);
	sprite.setTexture(temp, true);
	rt[current].draw(sprite);
	rt[current].display();
	sprite.setTexture(rt[current].getTexture());
	// link shader to current texture
	shader.setUniform("texture", sf::Shader::CurrentTexture);
}

void CellAutoMachine::update()
{
	current = !current;
	rt[current].draw(sprite, &shader);
	rt[current].display();
	sprite.setTexture(rt[current].getTexture());
}

void CellAutoMachine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

const sf::Texture& CellAutoMachine::getTexture() const
{
	return rt[current].getTexture();
}
