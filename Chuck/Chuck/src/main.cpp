#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "Level.h"
#include "Player.h"
#include "Block.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define VIEW_WIDTHG 1280.0f
#define VIEW_HEIGHT 720.0f

void ResizeView(const sf::RenderWindow& window, sf::View& view){

	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

int main(){

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHUCK THE CHALK");
	sf::View view (sf::Vector2f (0.0f, 0.0f), sf::Vector2f (VIEW_WIDTHG, VIEW_HEIGHT) );

	window.setFramerateLimit(75);

	float deltaTime = 0.0f;
	sf::Clock clock;

	sf::Texture pj;
	pj.loadFromFile("sprt_2.png");

	Player player(sf::Vector2f(WINDOW_WIDTH - 10.0f, WINDOW_HEIGHT / 4.0f));
	player.SetTexture(pj);
	Block block(sf::Vector2f(WINDOW_WIDTH / 2.0f, (WINDOW_HEIGHT / 2.0f) + 32.0f));

	Level level;

	// Start the game loop
	while (window.isOpen()) {

		deltaTime = clock.restart().asSeconds();

		if (deltaTime > 1.0f / 20.0f)
			deltaTime = 1.0f / 20.0f;

		// Process events
		sf::Event event;

		while (window.pollEvent(event)) {

			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// Close window with Escape
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();

			// Resize event
			else if ((event.type == sf::Event::Resized))
				ResizeView(window, view);
		}
		
		// Update Player
		player.Move(deltaTime);
		
		//Collision

		sf::Vector2f direccion;
		std::vector<Block> potentials = level.get_potentials(player.GetPosition());

		for (int i = 0; i < potentials.size(); i++) {

			Collider col = player.GetCollider();

			if (potentials[i].GetCollider().CheckCollision(col, direccion, 1.0f))
				player.OnCollision(direccion);
		}

		//View
		view.setCenter(player.GetPosition());

		// Clear screen
		window.clear();
		window.setView(view);

		// Draw on screen
		player.Draw(window);
		level.Draw_lvl(window);
		
		// Update the window
		window.display();

	}
	return EXIT_SUCCESS;
}