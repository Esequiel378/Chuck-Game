#pragma once

#include "SFML/Graphics.hpp"
#include "Level.h"

class Collider {

	private:

		sf::RectangleShape& body;

	public:

		Collider(sf::RectangleShape& body) : body(body){}

		void Move(float dx, float dy) { return this->body.move(dx, dy); }
		bool CheckCollision(Collider& other, sf::Vector2f& direction, float push);

		sf::Vector2f GetPosition() { return this->body.getPosition(); }
		sf::Vector2f GetHalfSize() { return this->body.getSize() / 2.0f; }
};

bool Collider::CheckCollision(Collider& other, sf::Vector2f& direction, float push) {

	sf::Vector2f otherPosition = other.GetPosition();
	sf::Vector2f otherHalfSize = other.GetHalfSize();
	sf::Vector2f thisPosition = GetPosition();
	sf::Vector2f thisHalfSize = GetHalfSize();

	sf::Vector2f delta (otherPosition.x - thisPosition.x, otherPosition.y - thisPosition.y);
	sf::Vector2f intersect(abs(delta.x) - (otherHalfSize.x + thisHalfSize.x), abs(delta.y) - (otherHalfSize.y + thisHalfSize.y));

	if (intersect.x < 0.0f && intersect.y < 0.0f) {

		push = std::min(std::max(push, 0.0f), 1.0f);

		if (intersect.x > intersect.y) {

			if (delta.x > 0.0f) {
				//Rigth Collision
				Move(intersect.x * (1.0f - push), 0.0f);
				other.Move(-intersect.x * push, 0.0f);
				
				direction.x = 1.0f;
				direction.y = 0.0f;

			}else {
				//Left Collision
				Move(-intersect.x * (1.0f - push), 0.0f);
				other.Move(intersect.x * push, 0.0f);
				
				direction.x = -1.0f;
				direction.y = 0.0f;

			}

		}
		else {

			if (delta.y > 0.0f) {
				//Up Collision
				Move(0.0f, intersect.y * (1.0f - push));
				other.Move(0.0f, -intersect.y * push);

				direction.x = 0.0f;
				direction.y = 1.0f;

			}else {
				//Down Collision
				Move(0.0f, -intersect.y * (1.0f - push));
				other.Move(0.0f, intersect.y * push);

				direction.x = 0.0f;
				direction.y = -1.0f;
			}
		}

		return true;
	}

	return false;
}
