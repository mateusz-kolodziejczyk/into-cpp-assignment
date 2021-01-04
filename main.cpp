// ZombieArena.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Player.h"
#include "ZombieArena.h"
#include "textureHolder.h"

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

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;

	VertexArray background;

	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");

	int numZombies = 0;
	int numZombiesAlive = 0;
	Zombie* zombies = nullptr;
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


					arena.width = 500;
					arena.height = 500;
					arena.left = 0;
					arena.top = 0;

					int tileSize = generateLevel(background, arena);

					player.spawn(arena, resolution, tileSize);
					numZombies = 10;

					delete[] zombies;
					zombies = createHorde(numZombies, arena);
					numZombiesAlive = numZombies;
					clock.restart();
				}

			}

		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (state == State::PLAYING) {
			if (Keyboard::isKeyPressed(Keyboard::W)) {
			}
			else {
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
			}
			else {
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
			}
			else {
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
			}
			else {
			}
		}

		// Update the frame
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			
			mouseScreenPosition = Mouse::getPosition(window);
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());
			mainView.setCenter(player.getCenter());

			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			
		}
		//Draw the scene
		if (state == State::PLAYING) {
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);
			window.draw(player.getSprite());
			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			if (state == State::PAUSED) {

			}
			if (state == State::GAME_OVER) {

			}
			window.display();
		}
	}
	delete[] zombies;
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
