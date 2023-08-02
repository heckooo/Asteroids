#pragma once
#include "GameObject.h"
#include "Projectile.h"
#include <list>
#include <thread>
#include <chrono>
#include <atomic>
#include <SFML/Audio.hpp>

class Player : public GameObject {
private:
	bool flying = false;
	std::chrono::time_point<std::chrono::system_clock> lastFlied = std::chrono::system_clock::now() - std::chrono::seconds(5);

	sf::Texture* projectile_texture;
	sf::Image* projectile_image;
	std::atomic<bool> canShoot{ true };
	std::thread reloadThread;
	std::atomic<std::chrono::time_point<std::chrono::system_clock>> lastShot{ std::chrono::system_clock::now() };
	std::atomic<bool> isReloading = false;

	double reloadTime = 0.2;
	double flyInterval = 3;
	double flyTime = 0.5;
	sf::Sound shootSound;
	sf::Sound flySound;
public:
	Player() = default;
	Player(sf::Texture& texture, sf::Image& image, float scale, float angle, float x_pos, float y_pos, sf::Texture& projectile_texture, sf::Image& projectile_image, int hitPoints) :
		GameObject(texture, image, scale, angle, x_pos, y_pos, hitPoints), projectile_texture(&projectile_texture), projectile_image(&projectile_image), reloadThread(std::thread(&Player::Reload, this)), isReloading(true) {
	}
	~Player();

	void Reload();
	void Shoot(std::list<Projectile>& projectiles, sf::SoundBuffer& shoot);
	void Fly(sf::SoundBuffer& thrust);
	void Stop();
	void Update() override;
};