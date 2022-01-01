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
	if (!playerTex.loadFromFile("textures/player.png")) {
		throw "could not laod astroid.png";
	}
	//set astroid texture
	this->astroid.setTexture(this->astroidTex);
	this->astroid.scale(0.35f, 0.35f);
	//set player texture
	this->player.setTexture(this->playerTex);
	this->player.scale(0.35f, 0.35f);
	this->player.setOrigin(this->player.getGlobalBounds().width , this->player.getGlobalBounds().height/2);
	this->playerAngle = 0.f;
	this->lose = false;
	//player position is set in initWindow
	//init projectiles 
	this->projectile.setSize(sf::Vector2f(5.f,10.5f));
	this->projectile.setFillColor(sf::Color::Red);
	this->fireLimit = true;
	
}

void Game::initWindow()
{
	//Game* e;
	//e->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Close);
	//cleaner verson of below
	this->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Default);
	this->window->setMouseCursorGrabbed(true);

	this->window->setFramerateLimit(60);

	//sets player start position
	this->player.setPosition(this->window->getSize().x / 2 - player.getGlobalBounds().width, this->window->getSize().y / 2 - player.getGlobalBounds().height);

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
void Game::PlayerControl()
{
	//rotate to face mouse
	/*
	is likley causeing crashes
	*/
	sf::Vector2i MousePos = sf::Mouse::getPosition(*(window));
	float angle = atan2( this->player.getPosition().y-MousePos.y, this->player.getPosition().x-MousePos.x  );
	angle = (angle* 180.f) / 3.141f;
	this->player.setRotation(angle-90);
	this->playerAngle = this->player.getRotation();
	//keyboard inputs
   //checks for keypress and keeps position of object on screen
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->player.getPosition().x - this->player.getGlobalBounds().width / 2 > 0) {
		this->player.move(-8.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->player.getPosition().x  + this->player.getGlobalBounds().width/2 < this->window->getSize().x) {
		this->player.move(8.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->player.getPosition().y - this->player.getGlobalBounds().height / 2 > 0) {
		this->player.move(0.f, -8.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->player.getPosition().y + this->player.getGlobalBounds().height/2 < this->window->getSize().y) {
		this->player.move(0.f, 8.f);
	}
	//mouse inputs and fire inputs
	/*
	No idea why the fireLimit and fireTimer break aiming
	*/
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		shootProjectiles(angle);
		this->fireLimit = false;
	}

	//checks for inpact of player and astroid
	for (size_t i = 0; astroids.size() > i; i++) {
		if (astroids[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
			this->lose = true;
			break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
		}
	}

	this->window->draw(this->player);
}

void Game::shootProjectiles(const float& angle)
{
		this->projectile.rotate(angle);
		this->projectile.setPosition(this->player.getPosition());
		sf::Vector2i mousePos = sf::Mouse::getPosition(*(window));
		sf::Vector2f playerPos = this->player.getPosition();
		//find slope for projectile to follow 
		float moveY = (mousePos.y - playerPos.y);
		float moveX = (mousePos.x - playerPos.x);
		projectieles.push_back(projectile);
		projectielePathX.push_back(moveX);
		projectielePathY.push_back(moveY);
}

void Game::updateProjectiles()
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
		else if(projectieles[i].getPosition().y < 0 || projectieles[i].getPosition().y > this->window->getSize().y){
			projectieles.erase(projectieles.begin() + i);
			projectielePathX.erase(projectielePathX.begin() + i);
			projectielePathY.erase(projectielePathY.begin() + i);
			continue;
		}
		else {
		projectieles[i].move(projectielePathX[i]/20,projectielePathY[i]/20);
		this->window->draw(projectieles[i]);
		}

		//check for colllision between the projectiles and astroids
		for (size_t j = 0; astroids.size() > j; j++) {
			if (projectieles[i].getGlobalBounds().intersects(astroids[j].getGlobalBounds())) {
				projectieles.erase(projectieles.begin() + i);
				astroids.erase(astroids.begin() + j);
				break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
			}
		}
	}
	

}

void Game::createEnemies()
{
	//create new astroids
	static short int spawnTimer = 80;
	if (spawnTimer > 40) {
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
		if (astroids[i].getPosition().x -901> this->window->getSize().x - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y -901> this->window->getSize().y - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().x +901< 0 - astroid.getGlobalBounds().width) {
			astroids.erase(astroids.begin() + i);
		}
		else if (astroids[i].getPosition().y +901< 0 - astroid.getGlobalBounds().height) {
			astroids.erase(astroids.begin() + i);
		}
	}
	
	//draw the astroids 
	for (size_t i = 0; i < astroids.size(); i++) {
		//find the slope
		float currentLocation[2] = {astroids[i].getPosition().x,astroids[i].getPosition().y};
		float slopeY = this->window->getSize().y/2 +100- currentLocation[0]/2;
		float slopeX = this->window->getSize().x/2 +100 - currentLocation[1]/2;
		float slope = slopeY / slopeX;
		
		//find next point on line, point slope form
		//curent location 1 is the x cord, loaction 0 is y
		float nextY = -1 * (slope * ((currentLocation[1] - 4.f) - currentLocation[1])) + currentLocation[0];
		
		astroids[i].move(4.f, (nextY - currentLocation[0]));
		this->window->draw(this->astroids[i]);

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
	
	if(lose == false){
		this->window->clear(sf::Color::White);
		//draw stuff
		//draw player
		PlayerControl();
		//make and draw astroids 
		createEnemies();
		//update projectiles
		updateProjectiles();
	}
	else {
		this->window->clear(sf::Color::Red);
		//create lose function
	}
	
	

	this->window->display();
}


