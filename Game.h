#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <vector>

class Enemy {
private:
	sf::Texture astroidTex;
	sf::Sprite astroid;
	
protected:
	std::vector<sf::Sprite> astroids;

public:
	Enemy();
	virtual ~Enemy();
	void createEnemies(sf::Vector2u size);
	void draw(sf::RenderWindow* window);
	sf::FloatRect getBounds() const;
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

	// colliders
	Player* player;
	std::vector<Enemy> enemies;

	//private functions
	void initializeVariables();
	void initWindow();
	
	//window pointer
	sf::RenderWindow* window;
	//win / loss condition
	bool lose;
	
public:
	//constructors / destructors
	Game();
	virtual ~Game();
	//accessor
	const bool getWindowIsOpen();
	//functions 
	void pollEvents();
	void update();
	void render() const;
	//lose function
	void Lose();
};
