#pragma once //cpp0x.pl/kursy/Kurs-SFML-2-x-C++
#include <SFML/Graphics.hpp>

class GameObject {
private: 
	sf::Image& image;
	int hitPoints;
protected: 
	float x, y;
	float scale;
	sf::Sprite sprite;
	sf::Vector2f velocity;
public: 
	GameObject() = default;
	GameObject(sf::Texture& texture, sf::Image& image, float scale, float angle, float x_pos, float y_pos, int hitPoints, sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f));

	float angle;
	 
	int getHitPoints() const { return hitPoints; };

	void Draw(sf::RenderWindow& window);
	virtual void Update();
	bool CollidesWith(const GameObject& other) const;
	bool PixelPerfectCollision(const sf::Sprite& a, const sf::Sprite& b,
		const sf::Image& imgA, const sf::Image& imgB) const;
	void Hit();
	bool isAlive() const;
	bool TouchesMapEdge(bool pushAway = false, float max_offset_x = 0, float max_offset_y = 0);
};