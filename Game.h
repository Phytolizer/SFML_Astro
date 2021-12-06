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
	std::vector<sf::Sprite> astroids;
	sf::Texture astroidTex;
	sf::Sprite astroid;
public:
	//constructors / destructors
	Game();
	virtual ~Game();
	//game object function 
	void createEnemies();


	//accesor
	const bool getWindowIsOpen();

	//functions 
	void pollEvents();
	void update();
	void render();
};

