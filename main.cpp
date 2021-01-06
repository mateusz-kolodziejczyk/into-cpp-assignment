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
#include <sstream>
#include <fstream>

using namespace sf;
int main()
{
	srand(time(0));
	enum class State { PAUSED, GAME_OVER, PLAYING, SPLASH_SCREEN };

	State state = State::SPLASH_SCREEN;

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
	int hiScore = 0;
	int score = 0;
	float timeRemaining = 30;

	// Splash SCreen
	Sprite splashScreen;
	Texture textureSplashScreen = TextureHolder::GetTexture("graphics/hud/background.png");
	splashScreen.setTexture(textureSplashScreen);
	splashScreen.setPosition(0, 0);

	// Image is 1920x1080, scale it based on resolution
	splashScreen.setScale(resolution.x / 1920, resolution.y / 1080);

	View hudView(FloatRect(0, 0, resolution.x, resolution.y));

	// Text
	Font font;
	font.loadFromFile("fonts/SBIN.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(150);
	pausedText.setFillColor(Color::White);
	pausedText.setString("Press Enter \nto Continue");

	// Place is middle of screen
	FloatRect pausedRect = pausedText.getLocalBounds();
	pausedText.setOrigin(pausedRect.left + pausedRect.width / 2.0f, pausedRect.top + pausedRect.height / 2.0f);
	pausedText.setPosition(resolution.x / 2, resolution.y / 2);

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setString("Press Enter to play");

	FloatRect gameOverRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(gameOverRect.left + gameOverRect.width / 2.0f, gameOverRect.top + gameOverRect.height / 2.0f);
	gameOverText.setPosition(resolution.x / 2, resolution.y / 2);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	std::ifstream inputFile("scores.txt");
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(resolution.x - 400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(resolution.x - 400, resolution.y - 100);
	zombiesRemainingText.setString("Zombies: 100");

	// Time Remaining
	
	Text timeRemainingText;
	timeRemainingText.setFont(font);
	timeRemainingText.setCharacterSize(55);
	timeRemainingText.setFillColor(Color::White);
	timeRemainingText.setPosition(resolution.x - 400, resolution.y - 200);
	timeRemainingText.setString("Time Remaining: 30");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(resolution.x * 0.6, resolution.y - 100);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(resolution.x * 0.05, resolution.y - 100);


	//Time since last hud update
	int framesSinceLastHudUpdate = 0;

	// How often should the hud be updated
	int fpsMeasurementFrameInterval = 30;

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
				else if (event.key.code == Keyboard::Return && (state == State::SPLASH_SCREEN || state == State::GAME_OVER)) {
					for (auto zombie : zombies) {
						delete(zombie);
					}
					zombies.clear();
					bullets.clear();
					zombieSpawners.clear();
					state = State::PLAYING;

					wave++;

					arena.width = 1000;
					arena.height = 850;
					arena.left = 0;
					arena.top = 0;
					zombiesLeftToSpawn = 6;
					timeRemaining = 20;

					tileSize = generateLevel(background, arena);

					player.spawn(arena, resolution, tileSize);
					mainView.setCenter(arena.width / 2, arena.height / 2);

					std::set<int> zombieTypes = { 0 };
					auto rightSpawner = ZombieSpawner(Vector2f(arena.width - tileSize * 1.5, tileSize * 2.5), 6, zombieTypes, true);
					auto leftSpawner = ZombieSpawner(Vector2f(tileSize * 1.5, tileSize * 2.5), 6, zombieTypes, false);
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
				// 1 = right, -1 = left
				bool facingRight = player.getDirection() > 0 ? true : false;
				auto bullet = Bullet(player.getCenter().x + player.getDirection() * (player.getPosition().width / 2), player.getCenter().y + 10, facingRight, arena);
				bullets.push_back(bullet);
				lastShot = gameTimeTotal;
			}

		}
		// Update the frame
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			timeRemaining -= dtAsSeconds;
			if (timeRemaining <= 0) {
				state = State::GAME_OVER;
			}


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
							// Update score/highscore
							score += 50;
							if (score >= hiScore) {
								hiScore = score;
							}
							float wavePenalty = wave * 0.2;
							if (wavePenalty >= 1.5) {
								wavePenalty = 1.5;
							}
							timeRemaining += 3 - wavePenalty;
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
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}// End player touched

			// The wave is cleared
			if (zombiesLeftToSpawn <= 0 && zombies.size() <= 0) {
				wave++;
				zombiesLeftToSpawn = 6+wave*3;
				timeRemaining += 15;
				// Upgrade a spawner based on current wave
				if (wave % 2 == 0) {
					int zombieTypesAmount = zombieSpawners[0].getZombieTypesAmount();
					// Only add zombie types if the spawner doesnt have all of them
					if (zombieTypesAmount < 3) {
						zombieSpawners[0].addZombieType(zombieTypesAmount);
					}
					// Only change spawn interval is its above a certain number to not overwhelm the player
					float spawnInterval = zombieSpawners[0].getSpawnInterval();
					if (spawnInterval > 4) {
						zombieSpawners[0].setSpawnInterval(spawnInterval - 0.5);
					}
				}
				else {
					int zombieTypesAmount = zombieSpawners[1].getZombieTypesAmount();
					if (zombieTypesAmount < 3) {
						zombieSpawners[1].addZombieType(zombieTypesAmount);
					}
					float spawnInterval = zombieSpawners[1].getSpawnInterval();
					if (spawnInterval > 4) {
						zombieSpawners[1].setSpawnInterval(spawnInterval - 0.5);
					}
				}
			}
			// If there are no zombies on the screen, accelerate zombie spawns
			else if (zombiesLeftToSpawn > 0 && zombies.size() <= 0) {
				for (auto iter = zombieSpawners.begin(); iter != zombieSpawners.end(); iter++) {
					if (iter->getSpawnTimer() <= iter->getSpawnInterval() * 0.8) {
						iter->setSpawnTimer(iter->getSpawnInterval() * 0.9);
					}
				}
			}
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));
			framesSinceLastHudUpdate++;

			if (framesSinceLastHudUpdate > fpsMeasurementFrameInterval) {
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesRemaining;
				std::stringstream ssTimeRemaining;

				// Update score text
				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());

				// Update the highscore text
				ssHiScore << "Hi Score: " << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Update the wave
				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());

				// Update the zombies remaining text
				ssZombiesRemaining << "Zombies Remaining: " << zombiesLeftToSpawn + zombies.size();
				zombiesRemainingText.setString(ssZombiesRemaining.str());

				// Update the time remaining
				ssTimeRemaining << "Time Remaining: " << static_cast<int>(timeRemaining);
				timeRemainingText.setString(ssTimeRemaining.str());
				framesSinceLastHudUpdate = 0;
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

			window.setView(hudView);

			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
			window.draw(timeRemainingText);

		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			window.draw(splashScreen);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		if (state == State::SPLASH_SCREEN) {
			window.draw(splashScreen);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
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
