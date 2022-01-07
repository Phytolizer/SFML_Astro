#include "Game.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>

#include <cmath>

//--------------------------------------------------game---------------------------------------------------------------
void Game::initializeVariables()
{
	// non-window-related initialization
	this->lose = false;
	this->player = new Player(window->getSize());

	// N times
	enemies.push_back(Enemy());
	enemies.back().createEnemies(window->getSize());
	enemies.push_back(Enemy());
}

void Game::initWindow()
{
	//Game* e;
	//e->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Close);
	//cleaner verson of below
	this->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Default);
	this->window->setMouseCursorGrabbed(true);

	this->window->setFramerateLimit(60);
}

//constructor
Game::Game()
{
	this->initWindow();
	this->initializeVariables();
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



// public functions

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
	this->Lose();
}


void Game::render() const
{
	/*
	-clear old frame
	-render and display new window
	*/
	//static player p;
	if(lose == false){
		this->window->clear(sf::Color::White);
		//draw stuff
		//draw player
		//colider
		player->PlayerControl(window);
		//make and draw astroids 
		//enemy::createEnemies();
		//update projectiles
		player->updateProjectiles(window);
	}
	else {
		this->window->clear(sf::Color::Red);
		//create lose function
	}
	
	

	this->window->display();
}

void Game::Lose() {
	if (this->player->checkCollision(this->enemies)) {
		lose = true;
	}
}

//--------------------------------------------player functions-----------------------------------------------------------

Player::Player(sf::Vector2u size)
{
	if (!playerTex.loadFromFile("textures/player.png")) {
		throw "could not load player.png";
	}
	//set player texture
	this->character.setTexture(this->playerTex);
	this->character.scale(0.5f, 0.5f);
	this->character.setOrigin(this->character.getGlobalBounds().width, this->character.getGlobalBounds().height / 2);
	//sets player start position
	this->character.setPosition(size.x / 2.f - character.getGlobalBounds().width, size.y / 2.f - character.getGlobalBounds().height);
	//init projectiles 
	this->projectile.setSize(sf::Vector2f(5.f, 10.5f));
	this->projectile.setFillColor(sf::Color::Red);
	frameCounter = 0;
}

Player::~Player()
{
}

void Player::PlayerControl(sf::RenderWindow* window) {
	
	//rotate to face mouse
	/*
	is likely causing crashes
	*/
	sf::Vector2i MousePos = sf::Mouse::getPosition(*(window));
	float angle = atan2(this->character.getPosition().y - MousePos.y, this->character.getPosition().x - MousePos.x);
	angle = (angle * 180.f) / 3.141f;
	this->character.setRotation(angle - 90);
	//keyboard inputs
   //checks for keypress and keeps position of object on screen
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->character.getPosition().x - this->character.getGlobalBounds().width / 2 > 0) {
		this->character.move(-8.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->character.getPosition().x + this->character.getGlobalBounds().width / 2 < window->getSize().x) {
		this->character.move(8.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->character.getPosition().y - this->character.getGlobalBounds().height / 2 > 0) {
		this->character.move(0.f, -8.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->character.getPosition().y + this->character.getGlobalBounds().height / 2 < window->getSize().y) {
		this->character.move(0.f, 8.f);
	}
	//mouse inputs and fire inputs
	++frameCounter;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && frameCounter > 20) {
		shootProjectiles(angle, window);
		frameCounter = 0;
	}

	window->draw(this->character);
}

void Player::shootProjectiles(const float& angle, sf::RenderWindow* window)
{
	this->projectile.rotate(angle);
	this->projectile.setPosition(this->character.getPosition());
	sf::Vector2i mousePos = sf::Mouse::getPosition(*(window));
	sf::Vector2f playerPos = this->character.getPosition();
	//find slope for projectile to follow 
	float moveY = (mousePos.y - playerPos.y);
	float moveX = (mousePos.x - playerPos.x);
	projectieles.push_back(projectile);
	projectielePathX.push_back(moveX);
	projectielePathY.push_back(moveY);
}

void Player::updateProjectiles(sf::RenderWindow* window)
{

	/*

	delete out of range projectiles

	*/
	for (size_t i = 0; i < projectieles.size(); i++) {
		if (projectieles[i].getPosition().x < 0 || projectieles[i].getPosition().x > window->getSize().x) {
			projectieles.erase(projectieles.begin() + i);
			projectielePathX.erase(projectielePathX.begin() + i);
			projectielePathY.erase(projectielePathY.begin() + i);
			continue;
		}
		else if (projectieles[i].getPosition().y < 0 || projectieles[i].getPosition().y > window->getSize().y) {
			projectieles.erase(projectieles.begin() + i);
			projectielePathX.erase(projectielePathX.begin() + i);
			projectielePathY.erase(projectielePathY.begin() + i);
			continue;
		}
		else {
			projectieles[i].move(projectielePathX[i] / 20, projectielePathY[i] / 20);
			window->draw(projectieles[i]);
		}

		
	}
}

bool Player::checkCollision(std::vector<Enemy> &enemies) {
	//check for colllision between the projectiles and astroids
	for (size_t i = 0; i < projectieles.size(); i++) {
		for (size_t j = 0; enemies.size() > j; j++) {
			if (projectieles[i].getGlobalBounds().intersects(enemies[j].getBounds())) {
				projectieles.erase(projectieles.begin() + i);
				enemies.erase(enemies.begin() + j);
				break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
			}
		}
	}
	//checks for inpact of player and astroid
	for (size_t i = 0; enemies.size() > i; i++) {
		if (enemies[i].getBounds().intersects(character.getGlobalBounds())) {
			//create a lose function
			return true;
		}
	}
	return false;
}

//-----------------------------------------------enemy ------------------------------------------------------------------------

Enemy::Enemy()
{
	//load the texture
	if (!astroidTex.loadFromFile("textures/astroid.png")) {
		throw "could not laod astroid.png";
	}

	//set astroid texture
	this->astroid.setTexture(this->astroidTex);
	this->astroid.scale(0.5f, 0.5f);
}

Enemy::~Enemy()
{
}

void Enemy::createEnemies(sf::Vector2u size)
{
	// TODO: Move to Game

	//create new astroids
	static short int spawnTimer = 80;
	if (spawnTimer > 80) {
		float randomSpawnX = (rand() % 900) * -1.f;
		float randomSpawnY = (rand() % 900) * -1.f;
		astroid.setPosition(sf::Vector2f(randomSpawnX, randomSpawnY));
		astroids.push_back(sf::Sprite(astroid));

		spawnTimer = 0;
	}
	else {
		spawnTimer++;
	}

	//check for out of bounds

	for (size_t i = 0; i < astroids.size(); i++) {
		if (astroids[i].getPosition().x - 901 > size.x - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y - 901 > size.y - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().x + 901 < 0 - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y + 901 < 0 - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
	}
}

void Enemy::draw(sf::RenderWindow* window) {
	//draw the astroids 
	for (size_t i = 0; i < astroids.size(); i++) {
		//find the slope
		float currentLocation[2] = { astroids[i].getPosition().x,astroids[i].getPosition().y };
		float slopeY = window->getSize().y / 2.f - currentLocation[0];
		float slopeX = window->getSize().x / 2.f - currentLocation[1];
		float slope = slopeY / slopeX;

		//find next point on line, point slope form
		//curent location 1 is the x cord, loaction 0 is y
		float nextY = -1 * (slope * ((currentLocation[1] - 4.f) - currentLocation[1])) + currentLocation[0];

		astroids[i].move(4.f, (nextY - currentLocation[0]));
		window->draw(this->astroids[i]);
	}
}

sf::FloatRect Enemy::getBounds() const {
	return this->astroid.getGlobalBounds();
}
