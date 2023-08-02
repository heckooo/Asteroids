#include "GameObject.h"
#include "settings.h"

GameObject::GameObject(sf::Texture& texture, sf::Image& image, float scale, float angle, float x_pos, float y_pos, int hitPoints, sf::Vector2f velocity) :
	scale(scale), x(x_pos), y(y_pos), angle(angle), velocity(velocity), image(image), hitPoints(hitPoints) {
	sprite.setTexture(texture);
	sprite.setRotation(angle);
	sprite.setScale(scale, scale);
	auto size = sprite.getTexture()->getSize();
	sprite.setOrigin(size.x / 2, size.y / 2);
	sprite.setPosition(x, y);
}

void GameObject::Draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

inline sf::IntRect FToIRect(const sf::FloatRect& f) {
	return sf::IntRect((int)f.left, (int)f.top, (int)f.width, (int)f.height);
}


bool GameObject::PixelPerfectCollision(const sf::Sprite& a, const sf::Sprite& b,
	const sf::Image& imgA, const sf::Image& imgB) const {
	sf::IntRect boundsA(FToIRect(a.getGlobalBounds()));
	sf::IntRect boundsB(FToIRect(b.getGlobalBounds()));
	sf::IntRect intersection;

	if (boundsA.intersects(boundsB, intersection)) {
		const sf::Transform& inverseA(a.getInverseTransform());
		const sf::Transform& inverseB(b.getInverseTransform());

		const sf::Vector2u& sizeA(imgA.getSize());
		const sf::Vector2u& sizeB(imgB.getSize());

		const sf::Uint8* pixA = imgA.getPixelsPtr();
		const sf::Uint8* pixB = imgB.getPixelsPtr();

		sf::Vector2f vecA, vecB;
		int xMax = intersection.left + intersection.width;
		int yMax = intersection.top + intersection.height;

		for (int x = intersection.left; x < xMax; x++)
			for (int y = intersection.top; y < yMax; y++) {
				vecA = inverseA.transformPoint(x, y);
				vecB = inverseB.transformPoint(x, y);

				int idxA = ((int)vecA.x + ((int)vecA.y) * sizeA.x) * 4 + 3;
				int idxB = ((int)vecB.x + ((int)vecB.y) * sizeB.x) * 4 + 3;

				if (vecA.x > 0 && vecA.y > 0 &&
					vecB.x > 0 && vecB.y > 0 &&
					vecA.x < sizeA.x && vecA.y < sizeA.y &&
					vecB.x < sizeB.x && vecB.y < sizeB.y &&
					pixA[idxA] > 0 &&
					pixB[idxB] > 0) {
					return true;
				}
			}
	}

	return false;
}

bool GameObject::CollidesWith(const GameObject& other) const {
	return PixelPerfectCollision(sprite, other.sprite, image, other.image);
}

void GameObject::Hit() {
	hitPoints--;
}

bool GameObject::isAlive() const {
	return hitPoints > 0;
}

bool GameObject::TouchesMapEdge(bool pushAway, float max_offset_x, float max_offset_y) {
	bool touches = false;
	// Top
	if (sprite.getGlobalBounds().top + max_offset_y <= 0) {
		if (pushAway) y += 5;
		touches = true;
	}
	// Bottom
	if (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height - max_offset_y >= settings::window_height) {
		if (pushAway) y -= 5;

		touches = true;
	}
	// Left
	if (sprite.getGlobalBounds().left + max_offset_x <= 0) {
		if (pushAway) x += 5;
		touches = true;
	}
	// Right
	if (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width - max_offset_x >= settings::window_width) {
		if (pushAway) x -= 5;
		touches = true;
	}

	return touches;
}


void GameObject::Update() {
	x += velocity.x;
	y += velocity.y;
	sprite.setPosition(x, y);
	sprite.setRotation(angle);
}