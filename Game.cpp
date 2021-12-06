#include "Game.h"

//private funcitons
void Game::initializeVariables()
{
	//init window
	this->window = nullptr;

	
	//load the texture
	if (!astroidTex.loadFromFile("textures/astroid.png")) {
		throw "could not laod astroid.png";
	}
	//set astroid texture
	this->astroid.setTexture(astroidTex);
}

void Game::initWindow()
{
	//Game* e;
	//e->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Close);
	//cleaner verson of below
	this->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Close);
	this->window->setFramerateLimit(144);
	
}

//constructor
Game::Game()
{
	this->initializeVariables();
	this->initWindow();
	
}

//destructor
Game::~Game()
{
	delete this->window;
}

//accessors

const bool Game::getWindowIsOpen()
{
	return this->window->isOpen();
}



// public fucntions
void Game::createEnemies()
{
	//create new astroids
	static short int spawnTimer = 80;
	if (spawnTimer > 80) {
		float randomSpawnX = (rand() % 100) * -1.f;
		float randomSpawnY = (rand() % 100) * -1.f;
		
		astroid.setPosition(sf::Vector2f(randomSpawnX, randomSpawnY));
		astroids.push_back(sf::Sprite(astroid));
		
		spawnTimer = 0;
	}
	else {
		spawnTimer++;
	}

	//check for out of bounds
	for (size_t i = 0; i < astroids.size(); i++) {
		if (astroids[i].getPosition().x > this->window->getSize().x - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);

		}
		else if (astroids[i].getPosition().y > this->window->getSize().y - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().x < 0 - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y < 0 - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
	}
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
        {
            //checks for someone clicking close button
            if (this->ev.type == sf::Event::Closed)
                this->window->close();
            //checks for keyboard input
            if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
            }
        }
}

void Game::update()
{
	this->pollEvents();
}


void Game::render()
{
	/*
	-clear old frame
	-render and display new window
	*/
	this->window->clear(sf::Color::Red);
	//draw stuff
	//this->window->draw(this->astroid);
	
	createEnemies();
	for (size_t i = 0; i < astroids.size(); i++) {
		//find the slope
		float currentLocation[2] = { this->astroids[i].getGlobalBounds().height, this->astroids[i].getGlobalBounds().width };
		float slopeY = this->window->getSize().y - currentLocation[0];
		float slopeX = this->window->getSize().x - currentLocation[1];
		float slope = slopeY / slopeX;
		std::cout << currentLocation[1] << "x, " << currentLocation[0] << "y" << std::endl;
		std::cout << slope << std::endl;
		
		//find next point on line
		//curent location 1 is the x cord, loaction 0 is y
		float nextY = (slope * ((currentLocation[1] - 5.f) - currentLocation[1])) - currentLocation[0];
		std::cout << nextY << "next y" << std::endl;
		astroids[i].move(5.f, nextY);
		this->window->draw(this->astroids[i]);
	}

	this->window->display();
}


