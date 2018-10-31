#pragma once

#include "SFML/Graphics.hpp"
#include "class_Block.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Level {

private:

	std::string name;
	std::wstring map;
	std::vector <Block> blocks;
	sf::Vector2i size;
	std::vector <sf::Vector2i> grid;

public:

	Level(sf::Vector2i size, const char* name);
	void AddBlock(sf::Vector2f pos, sf::Texture& texture);
	void DeleteBlock(sf::Vector2f pos);
	bool Save(sf::RenderWindow & window);
	void Draw(sf::RenderWindow& window) { for (int i = 0; i < this->blocks.size(); i++) { this->blocks[i].Draw(window); } }
};

Level::Level(sf::Vector2i size, const char* name) {

	size.x = abs(size.x / 16);
	size.y = abs(size.y / 16);


	this->size = size;
	this->name = name;

	this->map.resize(abs(size.x * size.y), '.');
	
}

void Level::AddBlock(sf::Vector2f pos, sf::Texture& texture) {


	pos.x = std::round(pos.x / 16) * 16;
	pos.y = std::round(pos.y / 16) * 16;

	//std::cout << this->blocks.size() << std::endl;

	bool in = false;

	for (int i = 0; i < this->blocks.size(); i++) {

		sf::Vector2f tPos = this->blocks[i].GetPosition();

		if (pos.x == tPos.x && pos.y == tPos.y) {
			in = true;
			break;
		}
	}

	if (!in) {
		
		Block block(pos);
		block.setTexture(texture);

		this->blocks.push_back(block);

		pos.x = abs(pos.x / 16);
		pos.y = abs(pos.y / 16);

		unsigned int index = pos.y * this->size.x + pos.x;
		this->map[index] = L'#';

	}
}

void Level::DeleteBlock(sf::Vector2f pos){

	pos.x = std::round(pos.x / 16) * 16;
	pos.y = std::round(pos.y / 16) * 16;

	//std::cout << this->blocks.size() << std::endl;

	for (int i = 0; i < this->blocks.size(); i++) {

		sf::Vector2f tPos = this->blocks[i].GetPosition();

		if (pos.x == tPos.x && pos.y == tPos.y) {

			unsigned int index = tPos.y * this->size.x + tPos.x;
			
			this->map[index] = L'.';
			this->blocks.erase(this->blocks.begin() + i);
			
			break;
		}
	}
}

bool Level::Save(sf::RenderWindow& window) {

	std::cout << "Saving...";

	std::ofstream newMap;

	newMap.open("maps/" + this->name + ".dat", std::ios::out);

	if (newMap.fail()) {

		std::cout << "Fail";
		return false;
	}

	for (int i = 0; i < this->size.y; i++) {
		for (int j = 0; j < this->size.y; j++) {

			unsigned int index = i * this->size.x + j;

			newMap << this->map[index];
		}
	}

	std::cout << "Success";

	newMap.close();
	return true;
}