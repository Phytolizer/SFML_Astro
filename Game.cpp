#include "Game.h"

//--------------------------------------------------game---------------------------------------------------------------
void Game::initializeVariables()
{
	//init window
	this->window = nullptr;
	this->lose = false;
	
}

void Game::initWindow()
{
	//Game* e;
	//e->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Close);
	//cleaner verson of below
	this->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Default);
	this->window->setMouseCursorGrabbed(true);

	this->window->setFramerateLimit(60);
	//sets loseptr
	
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
	
	if(lose == false){
		this->window->clear(sf::Color::White);
		//draw stuff
		//draw player
		//colider
		c.checkCollision();
		p.PlayerControl();
		//make and draw astroids 
		a.createEnemies();
		//update projectiles
		p.updateProjectiles();
	}
	else {
		this->window->clear(sf::Color::Red);
		//create lose function
	}
	
	

	this->window->display();
}

//--------------------------------------------player functions-----------------------------------------------------------

player::player()
{
	if (!playerTex.loadFromFile("textures/player.png")) {
		throw "could not laod astroid.png";
	}
	//set player texture
	this->character.setTexture(this->playerTex);
	this->character.scale(0.5f, 0.5f);
	this->character.setOrigin(this->character.getGlobalBounds().width, this->character.getGlobalBounds().height / 2);
	this->playerAngle = 0.f;
	//sets player start position
	this->character.setPosition(this->window->getSize().x / 2 - character.getGlobalBounds().width, this->window->getSize().y / 2 - character.getGlobalBounds().height);
	//init projectiles 
	this->projectile.setSize(sf::Vector2f(5.f, 10.5f));
	this->projectile.setFillColor(sf::Color::Red);
	this->fireLimit = true;
}

player::~player()
{
}

player::PlayerControl() {
	/*
	//rotate to face mouse
	
	sf::Vector2i MousePos = sf::Mouse::getPosition(*(window));
	float angle = atan2(this->character.getPosition().y - MousePos.y, this->character.getPosition().x - MousePos.x);
	angle = (angle * 180.f) / 3.141f;
	this->character.setRotation(angle - 90);
	this->playerAngle = this->character.getRotation();
	//keyboard inputs
   //checks for keypress and keeps position of object on screen
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->character.getPosition().x - this->character.getGlobalBounds().width / 2 > 0) {
		this->character.move(-8.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->character.getPosition().x + this->character.getGlobalBounds().width / 2 < this->window->getSize().x) {
		this->character.move(8.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->character.getPosition().y - this->character.getGlobalBounds().height / 2 > 0) {
		this->character.move(0.f, -8.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->character.getPosition().y + this->character.getGlobalBounds().height / 2 < this->window->getSize().y) {
		this->character.move(0.f, 8.f);
	}
	//mouse inputs and fire inputs
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		shootProjectiles(angle);
		this->fireLimit = false;
	}

	this->window->draw(this->character);
	*/
}

void player::shootProjectiles(const float& angle)
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

void player::updateProjectiles()
{

	/*

	delete out of range projectiles

	*/
	for (size_t i = 0; i < projectieles.size(); i++) {
		if (projectieles[i].getPosition().x < 0 || projectieles[i].getPosition().x > this->window->getSize().x) {
			projectieles.erase(projectieles.begin() + i);
			projectielePathX.erase(projectielePathX.begin() + i);
			projectielePathY.erase(projectielePathY.begin() + i);
			continue;
		}
		else if (projectieles[i].getPosition().y < 0 || projectieles[i].getPosition().y > this->window->getSize().y) {
			projectieles.erase(projectieles.begin() + i);
			projectielePathX.erase(projectielePathX.begin() + i);
			projectielePathY.erase(projectielePathY.begin() + i);
			continue;
		}
		else {
			projectieles[i].move(projectielePathX[i] / 20, projectielePathY[i] / 20);
			this->window->draw(projectieles[i]);
		}

		
	}
}

void player::Lose()
{
	Game::lose = true;
}


//-----------------------------------------------enemy ------------------------------------------------------------------------

enemy::enemy()
{
	//load the texture
	if (!astroidTex.loadFromFile("textures/astroid.png")) {
		throw "could not laod astroid.png";
	}

	//set astroid texture
	this->astroid.setTexture(this->astroidTex);
	this->astroid.scale(0.5f, 0.5f);
}

enemy::~enemy()
{
}

void enemy::createEnemies()
{
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
		if (astroids[i].getPosition().x - 901 > this->window->getSize().x - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y - 901 > this->window->getSize().y - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().x + 901 < 0 - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y + 901 < 0 - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
	}

	//draw the astroids 
	for (size_t i = 0; i < astroids.size(); i++) {
		//find the slope
		float currentLocation[2] = { astroids[i].getPosition().x,astroids[i].getPosition().y };
		float slopeY = this->window->getSize().y / 2 - currentLocation[0];
		float slopeX = this->window->getSize().x / 2 - currentLocation[1];
		float slope = slopeY / slopeX;

		//find next point on line, point slope form
		//curent location 1 is the x cord, loaction 0 is y
		float nextY = -1 * (slope * ((currentLocation[1] - 4.f) - currentLocation[1])) + currentLocation[0];

		astroids[i].move(4.f, (nextY - currentLocation[0]));
		this->window->draw(this->astroids[i]);

	}
}

void collider::checkCollision()
{
	//check for colllision between the projectiles and astroids
	for (size_t i = 0; projectieles.size(); i++) {
		for (size_t j = 0; astroids.size() > j; j++) {
			if (projectieles[i].getGlobalBounds().intersects(astroids[j].getGlobalBounds())) {
				projectieles.erase(projectieles.begin() + i);
				astroids.erase(astroids.begin() + j);
				break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
			}
		}
	}
	//checks for inpact of player and astroid
	for (size_t i = 0; astroids.size() > i; i++) {
		if (astroids[i].getGlobalBounds().intersects(character.getGlobalBounds())) {
			//create a lose function
			this->Lose();
			break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
		}
	}
}
