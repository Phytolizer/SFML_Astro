#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <complex>
#include <iostream>
#include "include/SFML/Graphics/RenderTarget.hpp"
#include "include/SFML/System/Vector2.hpp"
#include "include/SFML/Graphics.hpp"
#include "include/SFML/System.hpp"
#include "include/SFML/Window.hpp"

class Enemy {
private:
    sf::Texture astroidTex;
    sf::Sprite astroid;

public:
    Enemy();

    virtual ~Enemy();

    void draw(sf::RenderWindow *window);

    void createEnemies(const float& window_x, const float& window_y);
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
	void CreatePowerUp(const float& window_x, const float& window_y);
	void move(const float& x, const float& y);
	void draw(sf::RenderWindow* window);
	sf::Vector2f getPos();
	sf::FloatRect getBounds();
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
	bool checkCollision(std::vector<Enemy> &enemies, std::vector<powerUp>& power);
};


class Game
{
private:
	// events
	sf::Event ev;

	// collides
	Player* player;
	std::vector<Enemy> enemies;
	std::vector<powerUp> powerUps;
	//private functions
	void initializeVariables();
	void initWindow();
	
	//window pointer
	sf::RenderWindow* window;
	//win / loss condition
	bool lose;
	size_t frameCounter;
	size_t powerUpFrameCounter;
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
	void make_enemy(sf::Vector2u size);
	void make_powerUp(sf::Vector2u size);
	//lose function
	void Lose();
};
