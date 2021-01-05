// ZombieArena.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Player.h"
#include "ZombieArena.h"
#include "textureHolder.h"
#include "zombieSpawner.h"
#include "bullet.h"
#include "collision.h"
#include <set>

using namespace sf;
int main()
{
	srand(time(0));
	enum class State { PAUSED, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	TextureHolder holder;

	Vector2f resolution;
	resolution.x = 1980;
	resolution.y = 1080;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Default);

	View mainView(FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;

	VertexArray background;

	Texture textureBackground;
	textureBackground.loadFromFile("graphics/tilesheets/tiles.png");

	int zombiesLeftToSpawn = 0;
	int numZombiesAlive = 0;
	int tileSize = 0;
	std::vector<Zombie*> zombies;
	std::vector<Bullet> bullets;
	std::vector<ZombieSpawner> zombieSpawners;
	Time lastShot;
	float fireRate = 3;
	while (window.isOpen()) {
		//Handle input
		// Handle events

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::PLAYING;
					

					arena.width = 1000;
					arena.height = 850;
					arena.left = 0;
					arena.top = 0;
					zombiesLeftToSpawn = 10;

					tileSize = generateLevel(background, arena);

					std::cout << "\nTile size main: " << tileSize;
					player.spawn(arena, resolution, tileSize);
					mainView.setCenter(arena.width / 2, arena.height / 2);

					std::set<int> zombieTypes = { 0 };
					auto rightSpawner = ZombieSpawner(Vector2f(arena.width - tileSize * 1.5, tileSize * 2.5), 10, zombieTypes, true);
					auto leftSpawner = ZombieSpawner(Vector2f(tileSize * 1.5, tileSize * 2.5), 10, zombieTypes, false);
					zombieSpawners.push_back(rightSpawner);
					zombieSpawners.push_back(leftSpawner);
					clock.restart();
				}

			}

			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}

			if (state == State::PLAYING) {
				if (Keyboard::isKeyPressed(Keyboard::A)) {
					player.moveLeft();
				}
				else {
					player.stopLeft();
				}
				if (Keyboard::isKeyPressed(Keyboard::D)) {
					player.moveRight();
				}
				else {
					player.stopRight();
				}
				if (Keyboard::isKeyPressed(Keyboard::Space)) {
 					player.jump();
				}
			}
		}

		// Fire a bullet
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (gameTimeTotal.asMilliseconds() - lastShot.asMilliseconds() > 1000 / fireRate) {
				std::cout << "\nPosition: (" << player.getCenter().x << "," << player.getCenter().y << ")";
				// 1 = right, -1 = left
				bool facingRight = player.getDirection() > 0 ? true : false;
				auto bullet = Bullet(player.getCenter().x + player.getDirection() * (player.getPosition().width/2), player.getCenter().y+10, facingRight, arena);
				bullets.push_back(bullet);
				lastShot = gameTimeTotal;
			}

		}
		// Update the frame
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();


			mouseScreenPosition = Mouse::getPosition(window);
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition(), background);

			Vector2f playerPosition(player.getCenter());
			// Handle spawning zombies
			for (auto it = zombieSpawners.begin(); it != zombieSpawners.end(); it++) {
				bool readyToSpawn = it->timerCheck(dtAsSeconds);
				if (readyToSpawn) {
					// Only spawn more if there are any left to spawn on the level
					if (zombiesLeftToSpawn > 0) {
						auto zombieToAdd = it->spawn();
						if (zombieToAdd != nullptr) {
							zombies.push_back(zombieToAdd);
							zombiesLeftToSpawn -= 1;
						}
					}
				}
			}

			for (auto zombie : zombies) {
				zombie->update(dt.asSeconds(), playerPosition, background, arena, tileSize);
			}


			for (auto it = bullets.begin(); it != bullets.end();) {
				bool hitWall = it->update(dtAsSeconds);
				if (hitWall) {
					it = bullets.erase(it);
				}
				else {
					++it;
				}
			}
			
			// Collision 
			
			for (auto bulletIter = bullets.begin(); bulletIter != bullets.end();) {
				bool collided = false;
				for (auto zombieIter = zombies.begin(); zombieIter != zombies.end();) {
					if (collision::collisionCheck(bulletIter->getPosition(), (*zombieIter)->getPosition())) {
						collided = true;
						if ((*zombieIter)->takeDamage()) {
							// Delete the zombie
							// Release the memeory from the zombie
							delete(*zombieIter);
							zombieIter = zombies.erase(zombieIter);
						}
						else {
							++zombieIter;
						}
					}
					else {
						++zombieIter;
					}
				}
				if (collided) {
					bulletIter = bullets.erase(bulletIter);
				}
				else {
					++bulletIter;
				}
			}

			// Have any zombies touched the player?
			for (auto zombie : zombies)
			{
				if (collision::collisionCheck(zombie->getPosition(), player.getPosition()))
				{
					if (player.hit(gameTimeTotal))
					{
						// More here later
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

					}
				}
			}// End player touched

			if (zombiesLeftToSpawn <= 0 && zombies.size() <= 0) {
				state = State::GAME_OVER;
			}

		}

		//Draw the scene
		if (state == State::PLAYING) {
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);
			window.draw(player.getSprite());

			for (auto zombie : zombies) {
			
				window.draw(zombie->getSprite());
			
			}


			for (auto bullet : bullets) {
				window.draw(bullet.getShape());
			}
			if (state == State::PAUSED) {

			}
			if (state == State::GAME_OVER) {

			}
			window.display();
		}
	}
	for (auto zombie : zombies) {
		delete(zombie);
	}
	zombies.clear();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
