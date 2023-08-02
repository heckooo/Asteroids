#pragma once 
#include "GameObject.h"

class Asteroid : public GameObject {
private:
	int points;
public:
	Asteroid(sf::Texture& texture, sf::Image& image, float scale, float angle, float x_pos, float y_pos, int hitPoints, int points, sf::Vector2f velocity) 
		: points(points),
		GameObject(texture, image, scale, angle, x_pos, y_pos, hitPoints, velocity) {}

	int getPoints() const { return points; }
};
