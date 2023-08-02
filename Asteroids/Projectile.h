#pragma once
#include "GameObject.h"

class Projectile : public GameObject {
public:
	Projectile(sf::Texture& texture, sf::Image& image, float scale, float angle, float x_pos, float y_pos, sf::Vector2f velocity) :
		GameObject(texture, image, scale, angle, x_pos, y_pos, 1, velocity) {}
};