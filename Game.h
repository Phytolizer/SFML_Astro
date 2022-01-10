#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <vector>
#include <iostream>
class Enemy {
private:
	sf::Texture astroidTex;
	sf::Sprite astroid;

public:
	Enemy();
	virtual ~Enemy();
	void draw(sf::RenderWindow* window);
	void createEnemies();
	sf::FloatRect get_bounds();
	sf::Vector2f get_pos();
	void move(const float &x,const float &y);
};

class powerUp{
private:
	sf::CircleShape boostObj;
	unsigned int boostLevel;
public:
	powerUp();
	virtual ~powerUp();
	void spawnPowerUp();
	void movePowerUp();
};
class Player {
private:
	sf::Texture playerTex;
	//projectiles
	sf::RectangleShape projectile;
	std::vector<float> projectielePathX;
	std::vector<float> projectielePathY;
	void shootProjectiles(const float& angle, sf::RenderWindow* window);
	std::vector<sf::RectangleShape> projectieles;
	sf::Sprite character;
	size_t frameCounter;

public:
	Player(sf::Vector2u size);
	virtual ~Player();
	void PlayerControl(sf::RenderWindow* window);
	void updateProjectiles(sf::RenderWindow* window);
	bool checkCollision(std::vector<Enemy> &enemies);
};

/*
	class that acts as game engine
	wrapper class
*/
class Game
{
private:
	// events
	sf::Event ev;

	// collides
	Player* player;
	std::vector<Enemy> enemies;

	//private functions
	void initializeVariables();
	void initWindow();
	
	//window pointer
	sf::RenderWindow* window;
	//win / loss condition
	bool lose;
	size_t frameCounter;
	
public:
	//constructors / destructors
	Game();
	virtual ~Game();
	//accessor
	const bool getWindowIsOpen();
	//functions
	void pollEvents();
	void make_enemy(sf::Vector2u size);
	void update();
	void render();
	//lose function
	void Lose();
};
