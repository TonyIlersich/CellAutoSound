#pragma once

#include "includes.h"

// requires n > 0
template <unsigned int n>
class RenderBuffer : public sf::Drawable
{
	mutable sf::RenderTexture rt[n];
	const sf::Drawable* source = nullptr;

public:

	inline RenderBuffer(unsigned int width, unsigned int height)
	{
		for (auto& tex : rt) tex.create(width, height);
	}

	inline void setSource(const sf::Drawable& source)
	{
		this->source = &source;
	}

	inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::Sprite temp;
		temp.setTexture(rt[0].getTexture(), true);
		target.draw(temp);

		for (int i = 1; i < n; i++)
		{
			temp.setTexture(rt[i].getTexture(), true);
			rt[i - 1].draw(temp);
			rt[i - 1].display();
		}

		rt[n - 1].draw(*source);
		rt[n - 1].display();
	}
};

