#pragma once

#include "SFML/Graphics.hpp"
#include "Collider.h"

class Block{

	private:

		sf::RectangleShape body;

	public:

		Block(sf::Vector2f pos, sf::Vector2f size, sf::Color color);

		Collider GetCollider() { return Collider(this->body); }

		sf::Vector2f get_pos();

		void Draw(sf::RenderWindow & window);

};

Block::Block(sf::Vector2f pos,
	sf::Vector2f size = sf::Vector2f(16.0f, 16.0f),
	sf::Color color = sf::Color(4, 48, 4, 255)) {

	sf::Vector2f center(size.x / 2.0f, size.y / 2.0f);
	this->body.setSize(size);
	this->body.setOrigin(center);
	this->body.setPosition(pos);
	this->body.setFillColor(color);

}

void Block::Draw(sf::RenderWindow& window) {

	window.draw(this->body);
}

sf::Vector2f Block::get_pos() {

	return this->body.getPosition();
}