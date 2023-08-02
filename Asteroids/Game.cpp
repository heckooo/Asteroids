// Logika gry
#define _USE_MATH_DEFINES
#include "Game.h"
#include "settings.h"
#include <iostream>
#include <filesystem>
#include <exception>
#include <random>
#include <cmath>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Game::Game(int width, int height, sf::RenderWindow& window) : width(width), height(height), window(window) {

}

int Random(int a, int b) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist(a, b);
	return dist(rng);
}

void Game::Initialize() {
	asteroids = std::list<Asteroid>(); // Clearing lists
	projectiles = std::list<Projectile>();
	
	if (!player_texture.second.loadFromFile("assets/pixel_ship_yellow.png")) throw std::runtime_error("Failed to load texture");
	if (!projectile_texture.second.loadFromFile("assets/laser.png")) throw std::runtime_error("Failed to load texture");
	if (!asteroid_textures[0].second.loadFromFile("assets/a10015.png")) throw std::runtime_error("Failed to load texture");
	if (!asteroid_textures[1].second.loadFromFile("assets/c10015.png")) throw std::runtime_error("Failed to load texture");
	if (!font.loadFromFile("assets/arial.ttf")) throw std::runtime_error("Failed to load font");
	if (!hit.loadFromFile("assets/hit.wav")) throw std::runtime_error("Failed to load sound");
	if (!shoot.loadFromFile("assets/shoot.wav")) throw std::runtime_error("Failed to load sound");
	if (!thrust.loadFromFile("assets/thrust.wav")) throw std::runtime_error("Failed to load sound");
	if (!music.loadFromFile("assets/spacesound.wav")) throw std::runtime_error("Failed to load sound");

	musicSound.setBuffer(music);
	musicSound.setVolume(10.0f);
	musicSound.setLoop(true);
	musicSound.play();

	player_texture.first.loadFromImage(player_texture.second);
	projectile_texture.first.loadFromImage(projectile_texture.second);

	asteroid_textures[0].first.loadFromImage(asteroid_textures[0].second);
	asteroid_textures[1].first.loadFromImage(asteroid_textures[1].second);

	player = std::make_unique<Player>(player_texture.first, player_texture.second, 1.0f, 0.0f, 
		settings::window_width / 2, settings::window_height / 2, projectile_texture.first, projectile_texture.second, settings::player_hit_points); // Creating player

	points = 0;
	inProgress = true;
}

void Game::CreateAsteroid(float x_pos, float y_pos, sf::Vector2f astVel) {
	int rand = Random(0, asteroid_textures.size() - 1);
	float rand_angle = Random(0, 360);
	auto& asteroid_texture_image = asteroid_textures[rand];

	asteroids.push_back(Asteroid(asteroid_texture_image.first, asteroid_texture_image.second, 1.0f, 
		rand_angle, x_pos, y_pos, asteroid_texture_image.first.getSize().x < 90 ? 1 : 2, asteroid_texture_image.first.getSize().x < 90 ? 10 : 20, astVel));
}

void Game::Draw() {
	window.clear();

	if (!inProgress) {
		sf::Text score;
		score.setFont(font);
		score.setPosition(sf::Vector2f(settings::window_width / 2 - 100.0f, settings::window_height / 2 - 50.0f));
		score.setString("Your score: " + std::to_string(points) + "\nPress esc to quit\nPress enter to restart");
		score.setFillColor(sf::Color::White);
		score.setStyle(sf::Text::Bold);

		window.draw(score);
	}
	else {
		player->Draw(window);
		for (Asteroid& asteroid : asteroids) {
			asteroid.Draw(window);
		}
		for (Projectile& projectile : projectiles) {
			projectile.Draw(window);
		}

		sf::Text text;
		sf::Text lives;

		text.setFont(font);
		lives.setFont(font);
		lives.setPosition(sf::Vector2f(settings::window_width - 170, 0.0f));
		lives.setString("Lives left: " + std::to_string(player->getHitPoints()));
		text.setString("Points: " + std::to_string(points));

		text.setCharacterSize(30);
		lives.setCharacterSize(30);

		text.setFillColor(sf::Color::White);
		lives.setFillColor(sf::Color::White);

		text.setStyle(sf::Text::Bold);
		lives.setStyle(sf::Text::Bold);

		window.draw(lives);
		window.draw(text);
	}
	window.display();
}

void Game::Play() {
	while (inProgress || !quit) {
		Update();
		Draw();
	}
}

void Game::Turn(Direction direction) {
	if (direction == Direction::Right) {
		player->angle += 1;
	}
	if (direction == Direction::Left) {
		player->angle -= 1;
	}
}

void Game::KeyHandler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		player->Shoot(projectiles, shoot);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		Turn(Direction::Left);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		Turn(Direction::Right);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		player->Fly(thrust);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && !inProgress) {
		quit = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !inProgress) {
		Initialize();
	}
}

void Game::Spawn() {
	// Zakladamy, ze 1 to sciana gorna, nastepne wg wskazowek zegara
	int wall = Random(1, 4);
	float vertical_offset = Random(50, 200);
	float horizontal_offset = Random(0, wall %2 == 0 ? settings::window_height : settings::window_width);
	float randPoint = Random(0, wall % 2 == 0 ? settings::window_height : settings::window_width);
	float randSpeed = Random(50, 150) * settings::asteroid_velocity * 0.01;

	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = now - lastAsteroid;

	if (elapsed_seconds.count() <= asteroidInterval) {
		return;
	}
	else {
		lastAsteroid = now;
	}

	sf::Vector2f asteroidPos(0.0f, 0.0f);
	sf::Vector2f target(0.0f, 0.0f);

	switch (wall) {
	case 1:
		asteroidPos = sf::Vector2f(horizontal_offset, -vertical_offset);
		target = sf::Vector2f(randPoint, settings::window_height);
		break;
	case 2:
		asteroidPos = sf::Vector2f(settings::window_width + vertical_offset, horizontal_offset);
		target = sf::Vector2f(0, randPoint);
		break;
	case 3:
		asteroidPos = sf::Vector2f(horizontal_offset, settings::window_height + vertical_offset);
		target = sf::Vector2f(randPoint, 0);
		break;
	case 4:
		asteroidPos = sf::Vector2f(-vertical_offset, horizontal_offset);
		target = sf::Vector2f(settings::window_width, randPoint);
		break;
	}

	float angle = std::atan2(target.y - asteroidPos.y, target.x - asteroidPos.x) * (180 / M_PI);
	sf::Vector2f astVel(sin(angle) * randSpeed, cos(angle) * -randSpeed);
	CreateAsteroid(asteroidPos.x, asteroidPos.y, astVel);
}

void Game::GameOver() {
	std::ofstream outfile;
	fs::path filePath = "scores.txt";

	if (fs::exists(filePath)) {
		outfile.open(filePath, std::ios_base::app);
		outfile << points << ",";
		outfile.close();
	}

	inProgress = false;
}

void Game::Update() {
	KeyHandler();

	if (!inProgress) return;

	player->Update();
	Spawn();
	

	for (auto asteroid = asteroids.begin(); asteroid != asteroids.end();) {
		asteroid->Update();

		if (asteroid->TouchesMapEdge(false, 330.0f, 330.0f)) {
			asteroids.erase(asteroid++);
		} else {
			++asteroid;
		}
	}
	for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
		projectile->Update();

		if (projectile->TouchesMapEdge(false, 20.0f, 20.0f)) {
			projectiles.erase(projectile++);
		}
		else {
			++projectile;
		}
	}



	for (auto ast = asteroids.begin(); ast != asteroids.end();) {
		bool removeAsteroid = false;
		for (auto proj = projectiles.begin(); proj != projectiles.end();) {	
			if (ast->CollidesWith(*proj)) {
				proj->Hit();
				ast->Hit();
				if (!ast->isAlive()) {
					hitSound.setBuffer(hit);
					hitSound.setVolume(40.0f);
					hitSound.play();
					removeAsteroid = true;
					points += ast->getPoints();
				}
			}

			if (!proj->isAlive()) {
				projectiles.erase(proj++);
			}
			else {
				++proj;
			}
		}
		if (ast->CollidesWith(*player) && !removeAsteroid) {
			removeAsteroid = true;
			player->Hit();
			hitSound.setBuffer(hit);
			hitSound.setVolume(40.0f);
			hitSound.play();
			if (!player->isAlive()) GameOver();
		}

		if (removeAsteroid) {
			asteroids.erase(ast++);
		}
		else {
			++ast;
		}
	}

	if (player->TouchesMapEdge(true)) player->Stop();
}
