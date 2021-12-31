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
protected:
	sf::RenderWindow* window;


private:
	//private variables
	// window
	sf::Event ev;
	//private functions
	void initializeVariables();
	void initWindow();
protected:
	//game entities
	//astroids
	std::vector<sf::Sprite> astroids;
	sf::Texture astroidTex;
	sf::Sprite astroid;
	//player
	sf::Texture playerTex;
	sf::Sprite player;
	bool lose;
	//projectiles
	float playerAngle;
	bool fireLimit;
	sf::RectangleShape projectile;
	std::vector<sf::RectangleShape> projectieles;
	std::vector<float> projectielePathX;
	std::vector<float> projectielePathY;
public:
	//constructors / destructors
	Game();
	virtual ~Game();
	//game object function 
	void createEnemies();
	void PlayerControl();
	void shootProjectiles(const float& angle);
	void updateProjectiles();
	//accesor
	const bool getWindowIsOpen();

	//functions 
	void pollEvents();
	void update();
	void render();
};

