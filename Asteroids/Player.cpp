#define _USE_MATH_DEFINES
#include "Player.h"
#include <cmath>

Player::~Player() {
	isReloading = false;
	if (reloadThread.joinable()) reloadThread.join();
}

void Player::Shoot(std::list<Projectile>& projectiles, sf::SoundBuffer& shoot) {
	if (!canShoot) return;
	float radians = (angle * M_PI) / 180;
	float offset_x = (sprite.getTexture()->getSize().y / 2) * std::sin(radians);
	float offset_y = (sprite.getTexture()->getSize().y / 2) * std::cos(radians);
	projectiles.push_back(Projectile(*projectile_texture, *projectile_image, 1.0f, this->angle, this->x + offset_x, 
		this->y - offset_y, sf::Vector2f(std::sin(radians)*2, std::cos(radians)*(-2))));
	lastShot = std::chrono::system_clock::now();
	canShoot = false;

	shootSound.setBuffer(shoot);
	shootSound.setVolume(8.0f);
	shootSound.play();
}

void Player::Reload() {
	while (isReloading) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (canShoot) continue;

		auto now = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = now - lastShot.load();

		if (elapsed_seconds.count() >= reloadTime) {
			canShoot = true;
		}
	}
}

void Player::Fly(sf::SoundBuffer& thrust) {
	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = now - lastFlied;

	if (elapsed_seconds.count() >= flyInterval) {
		flying = true;
		lastFlied = now;

		flySound.setBuffer(thrust);
		flySound.play();
	}
}

void Player::Stop() {
	velocity = sf::Vector2f(0.0f, 0.0f);
	flying = false;
}

void Player::Update() {
	if (flying) {
		float radians = (angle * M_PI) / 180;
		velocity.x = std::sin(radians) * 1;
		velocity.y = std::cos(radians) * (-1);

		auto now = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = now - lastFlied;
		if (elapsed_seconds.count() >= flyTime) Stop();
	}

	GameObject::Update();
}