#include <SFML/Graphics.hpp>
#include "class_Level.h"
#include <iostream>
#include <math.h>

#define PI 	3.14159265358979323846

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define VIEW_WIDTHG 1280.0f
#define VIEW_HEIGHT 720.0f

void ResizeView(const sf::RenderWindow& window, sf::View& view) {

	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

void AddBlocks(sf::RenderWindow& window, sf::View& view, Level& level, sf::Texture& texture) {

	// get the current mouse position in the window
	sf::Vector2i mPos = sf::Mouse::getPosition(window);

	//Map mouse position to pixel position
	sf::Vector2f nPos = window.mapPixelToCoords(mPos, view);

	level.AddBlock(nPos, texture);

}

void DelBlocks(sf::RenderWindow& window, sf::View& view, Level& level) {

	// get the current mouse position in the window
	sf::Vector2i mPos = sf::Mouse::getPosition(window);

	//Map mouse position to pixel position
	sf::Vector2f nPos = window.mapPixelToCoords(mPos, view);

	level.DeleteBlock(nPos);

}

int main() {

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHUCK THE CHALK");

	sf::View fixed = window.getView(); // Never change

	sf::View view = fixed; // Always displayed

	// Minimap
	unsigned int size = 200;
	sf::View minimap(sf::FloatRect(view.getCenter().x, view.getCenter().y, static_cast<float>(size),
		static_cast<float>(window.getSize().y * size / window.getSize().x)));

	minimap.setViewport(sf::FloatRect(1.f - static_cast<float>(minimap.getSize().x) / window.getSize().x - 0.02f,
		1.f - static_cast<float>(minimap.getSize().y) / window.getSize().y - 0.02f, static_cast<float>(minimap.getSize().x) / window.getSize().x,
		static_cast<float>(minimap.getSize().y) / window.getSize().y));

	minimap.zoom(4.f);

	//Rectangle behind the minimap
	sf::RectangleShape miniback;
	miniback.setPosition(minimap.getViewport().left * window.getSize().x - 5, minimap.getViewport().top * window.getSize().y - 5);
	miniback.setSize(sf::Vector2f(minimap.getViewport().width * window.getSize().x + 10, minimap.getViewport().height * window.getSize().y + 10));
	miniback.setFillColor(sf::Color(17, 38, 35, 255));

	//Inventory view
	sf::View viewInventory(sf::FloatRect(0.f, 0.f, static_cast<float>(window.getSize().x / 2), static_cast<float>(window.getSize().y)));
	viewInventory.setViewport(sf::FloatRect(0.1f, 0.1f, 0.8f, 0.8f));

	//Rectangle behind the inventory
	sf::RectangleShape inventback;
	inventback.setPosition(viewInventory.getViewport().left * window.getSize().x - 5, viewInventory.getViewport().top * window.getSize().y - 5);
	inventback.setSize(sf::Vector2f(viewInventory.getViewport().width * window.getSize().x + 10, viewInventory.getViewport().height * window.getSize().y + 10));
	inventback.setFillColor(sf::Color(45.5f, 45.9f, 45.9f, 230));

	//Create level
	Level level(sf::Vector2i(-10000, 10000), "main_level");

	//Fonts

	sf::Font invFont;
	if (!invFont.loadFromFile("fonts/arial.ttf"))
		std::cout << "Cant find the font file" << std::endl;

	sf::String sentence = "Inventory";

	//inventory texts
	sf::Text text(sentence, invFont, 30);
	text.setFillColor(sf::Color(44, 124, 255));
	text.setStyle(sf::Text::Bold);
	text.setOrigin(sentence.getSize() / 2 * 150, 40);
	text.setPosition(viewInventory.getSize().x / 2, 50);

	//Textures 578x578
	sf::Texture tBlocks;
	sf::Vector2u tSize = tBlocks.getSize();
	tSize.x /= 64;
	tSize.y /= 64;
	tBlocks.loadFromFile("resources/blocks/spr_blocks.png");

	window.setFramerateLimit(75);

	float deltaTime = 0.0f;
	bool isPressed = false;
	bool LControl = false;
	bool inWindow;
	bool showInventory = false;

	sf::Clock clock;
	sf::Vector2f oldPos;

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

			//Mouse leaves
			else if (event.type == sf::Event::MouseLeft) {
				inWindow = false;
			}

			//Mouse entered
			else if (event.type == sf::Event::MouseEntered) {
				inWindow = true;
			}

			// Resize event
			else if (event.type == sf::Event::Resized)
				ResizeView(window, view);

			//Middel mouse pressed
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Middle) {
					isPressed = true;
					sf::Vector2i mPosition = sf::Mouse::getPosition(window);
					oldPos = window.mapPixelToCoords(mPosition, view);
				}
			}

			//Middle mouse released
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Middle)
					isPressed = false;

			}

			//Mouse Moved
			else if (event.type == sf::Event::MouseMoved && !showInventory) { // MEJORAR MOPVIMIENTO DE CAMARA
				if (isPressed) {

					// Determine the new position in world coordinates
					sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

					sf::Vector2f move(0.0f, 15.0f);
					unsigned int Gap = 350 / move.y; 

					if ((newPos.x - oldPos.x) < Gap){
						//Mouse Left - Camera Rigth

						view.move(move.y, move.x);
						minimap.move(move.y, move.x);
					}

					if ((newPos.y - oldPos.y) < Gap) {
						//Mouse Down - Camera Up

						view.move(move.x, move.y);
						minimap.move(move.x, move.y);
					}

					if ((oldPos.x - newPos.x) < Gap) {
						//Mouse Rigth - Camera Left

						view.move(-move.y, move.x);
						minimap.move(-move.y, move.x);
					}

					if ((oldPos.y - newPos.y) < Gap) {
						//Mouse Up - Camera Down
	
						view.move(move.x, -move.y);
						minimap.move(move.x, -move.y);
					}

						oldPos = newPos;
				}
			}

			//Zoom
			else if (event.type == sf::Event::MouseWheelMoved && !showInventory){

				if (event.mouseWheel.delta == 1) {

					minimap.zoom(0.9);

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
						view.zoom(0.9);
					}
				}

				if (event.mouseWheel.delta == -1) {
	
					minimap.zoom(1.1);

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
						view.zoom(1.1);
					}
				}
			}

			//keyPressed
			else if (event.type == sf::Event::KeyPressed) {

				if (event.key.code == sf::Keyboard::LControl)
					LControl = true;

				if (event.key.code == sf::Keyboard::S && LControl)
					level.Save(window);

				if (event.key.code == sf::Keyboard::Tab)
					showInventory = !showInventory;
			}

			//KeyReleased
			else if (event.type == sf::Event::KeyReleased) {

				if (event.key.code == sf::Keyboard::LControl)
					LControl = false;

			}
			
		}//END OF EVENT HANDLE

		// Clear screen
		window.clear();

		if (!showInventory) {
		
			//Add blocks to screen
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && inWindow)
				AddBlocks(window, view, level, tBlocks);

			//Delete blocks
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && inWindow)
				DelBlocks(window, view, level);

		}

		//Draw main view
		window.setView(view);
		level.Draw(window);

		if(showInventory){

			//Draw back
			window.setView(fixed);
			window.draw(inventback);

			//Draw inventory
			window.setView(viewInventory);
			window.draw(text);

		}
		else {

			//Draw GUI
			window.setView(fixed);
			window.draw(miniback);

			//Draw minimap
			window.setView(minimap);
			level.Draw(window);
		}
		
		// Update the window
		window.display();

	}
	return EXIT_SUCCESS;
}