#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <vector>
/*
	class that acts as game engine
	wrapper class
*/
class Game
{
private:
	// events
	sf::Event ev;
	//private functions
	void initializeVariables();
	void initWindow();
	
protected:
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
	void render();
};


class player:Game {
private:
	sf::Texture playerTex;
	float playerAngle;
	//projectiles
	sf::RectangleShape projectile;
	std::vector<float> projectielePathX;
	std::vector<float> projectielePathY;
	void shootProjectiles(const float& angle);
protected:
	std::vector<sf::RectangleShape> projectieles;
	sf::Sprite character;

public:
	player();
	virtual ~player();
	void PlayerControl();
	void updateProjectiles();
	//lose function
	void Lose();
};

class enemy:Game {
private:
	sf::Texture astroidTex;
	sf::Sprite astroid;
	
protected:
	std::vector<sf::Sprite> astroids;

public:
	enemy();
	virtual ~enemy();
	void createEnemies();
};

class collider :player, enemy {
public:
	 void checkCollision();
};