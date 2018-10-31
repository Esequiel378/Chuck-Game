#pragma once


#include "SFML/Graphics.hpp"

class Block {

	private:
		
		sf::RectangleShape body;
		sf::Texture texture;

	public:

		Block(sf::Vector2f pos);
		void setTexture(sf::Texture & texture);
		void Draw(sf::RenderWindow& window) { window.draw(this->body); }
		sf::Vector2f GetPosition() { return this->body.getPosition(); }

};

Block::Block(sf::Vector2f pos){

	sf::Vector2f size(16.0f, 16.0f);
	sf::Vector2f center(size.x / 2, size.y / 2);

	this->body.setPosition(pos);
	this->body.setSize(size);
	this->body.setOrigin(center);
	this->body.setFillColor(sf::Color::Red);

}

void Block::setTexture(sf::Texture& texture) {

	this->texture = texture;
	this->body.setTexture(&texture);
}