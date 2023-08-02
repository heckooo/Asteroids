#pragma once
#include "Player.h"
#include "Asteroid.h"
#include "Projectile.h"
#include <list>
#include <vector>
#include <SFML/Audio.hpp>

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

class Game {
private:
	std::unique_ptr<Player> player;
	sf::RenderWindow& window;
	int width, height;
	std::list<Asteroid> asteroids;
	std::list<Projectile> projectiles;
	std::pair<sf::Texture, sf::Image> player_texture;
	std::pair<sf::Texture, sf::Image> projectile_texture;
	std::vector<std::pair<sf::Texture, sf::Image>> asteroid_textures = std::vector<std::pair<sf::Texture, sf::Image>>(2, std::make_pair(sf::Texture(), sf::Image()));;
	std::chrono::time_point<std::chrono::system_clock> lastAsteroid = std::chrono::system_clock::now();
	double asteroidInterval = 0.25;
	bool inProgress = false;
	bool quit = false;
	int points = 0;
	sf::Font font;
	sf::SoundBuffer hit;
	sf::SoundBuffer shoot;
	sf::SoundBuffer thrust;
	sf::SoundBuffer music;
	sf::Sound hitSound;
	sf::Sound musicSound;
public:
	Game(int width, int height, sf::RenderWindow& window);

	void Initialize();
	void Draw();
	void Play();
	void KeyHandler();
	void Update();
	void Turn(Direction direction);
	void Spawn();
	void CreateAsteroid(float x_pos, float y_pos, sf::Vector2f astVel);
	void GameOver();
};