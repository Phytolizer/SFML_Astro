#include <cmath>
#include "Game.h"
#include "include/SFML/Graphics/RenderWindow.hpp"
#include "include/SFML/System/Vector2.hpp"
#include "include/SFML/Window/Window.hpp"


//--------------------------------------------------game---------------------------------------------------------------
void Game::initializeVariables() {
    // non-window-related initialization
    this->lose = false;
    this->player = new Player(window->getSize());
    this->frameCounter = 80;
    this->powerUpFrameCounter = 800;

}

void Game::initWindow()
{
    this->window = new sf::RenderWindow(sf::VideoMode(900, 900), "Game 1", sf::Style::Default);
    this->window->setMouseCursorGrabbed(false);

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
    delete this->player;
}


//accessors



const bool Game::getWindowIsOpen()
{
    return this->window->isOpen();
}



// public functions

void Game::pollEvents()
{
    while (this->window->pollEvent(this->ev)) {
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


void Game::render()
{
    /*
    -clear old frame
    -render and display new window
    */

    if(lose == false){
        this->window->clear(sf::Color::Black);
        //draw stuff

        player->PlayerControl(window);
        //update projectiles
        player->updateProjectiles(window);

        this->make_enemy(window->getSize());
        this->make_powerUp(window->getSize());
    } else {
        this->window->clear(sf::Color::Red);
    }


    this->window->display();
}

void Game::make_enemy(sf::Vector2u size)
{
    ++frameCounter;
    //TODO: make so enemies ups that spawn on the positive x side of the screen move across the screen

    if(frameCounter >= 80) {
        Enemy enemy;
        enemy.createEnemies(window->getSize().x, window->getSize().y);
        enemies.emplace_back(std::move(enemy));
        frameCounter = 0;
    }
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        //get location
        const sf::Vector2f location = it->get_pos();

        if (location.x - 901 > size.x - location.x) {
            it = enemies.erase(it);
            continue;
        }else if (location.y - 901 > size.y - location.y) {
            it = enemies.erase(it);
            continue;
        }else if (location.x + 901 < size.x + location.x) {
            it = enemies.erase(it);
            continue;
        }else if (location.y + 901 < size.y + location.y) {
            it = enemies.erase(it);
            continue;
        }
        //find the slope

        float slopeY = size.y / 2.f - location.y;
        float slopeX = size.x / 2.f - location.x;
        float slope = slopeY / slopeX;
        std::cout << "slope: " << slope << std::endl;
        //find next point on line, point slope form
        //curent location 1 is the x cord, location 0 is y
        float nextY = -1 * (slope * ((location.x - 4.f) - location.x)) + location.y;

        it->move(4.f, (nextY - location.y));
        it->draw(this->window);
        ++it;
    }

}

void Game::make_powerUp(sf::Vector2u size)
{
    //TODO: make so power ups that spawn on the positive x side of the screen move across the screen
    ++powerUpFrameCounter;

    if (powerUpFrameCounter >= 800) {
        powerUps.push_back(powerUp());
        powerUps.back().CreatePowerUp(window->getSize().x,window->getSize().y);
        powerUpFrameCounter = 0;
    }
    for (size_t i = 0; i < powerUps.size(); i++) {
        //get location
        const sf::Vector2f location = this->powerUps[i].getPos();

        if (location.x - 901 > size.x - location.x) {
            powerUps.erase(powerUps.begin() + i);
            continue;
        }
        else if (location.y - 901 > size.y - location.y) {
            powerUps.erase(powerUps.begin() + i);
            continue;
        }
        else if (location.x + 901 < size.x + location.x) {
            powerUps.erase(powerUps.begin() + i);
            continue;
        }
        else if (location.y + 901 < size.y + location.y) {
            powerUps.erase(powerUps.begin() + i);
            continue;
        }
        //find the slope

        float slopeY = size.y / 2.f - location.y/2.f;
        float slopeX = size.x / 2.f - location.x/2.f;
        float slope = slopeY / slopeX;

        //find next point on line, point slope form
        //curent location 1 is the x cord, location 0 is y
        float nextY = -1 * (slope * ((location.x - 4.f) - location.x)) + location.y;

        powerUps[i].move(4.f, (nextY - location.y));
        powerUps[i].draw(this->window);
    }

}

void Game::Lose() {
    if (this->player->checkCollision(this->enemies,this->powerUps)) {
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
    sf::Vector2i MousePos = sf::Mouse::getPosition(*(window));
    float angle = atan2(this->character.getPosition().y - MousePos.y, this->character.getPosition().x - MousePos.x);
    angle = (angle * 180.f) / 3.141f;
    this->character.setRotation(angle - 90);

    //keyboard inputs
    //checks for keypress and keeps position of object on screen
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->character.getPosition().x - this->character.getGlobalBounds().width / 2.f > 0.f) {
        this->character.move(-8.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->character.getPosition().x + this->character.getGlobalBounds().width / 2.f < window->getSize().x) {
        this->character.move(8.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->character.getPosition().y - this->character.getGlobalBounds().height / 2.f > 0.f) {
        this->character.move(0.f, -8.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->character.getPosition().y + this->character.getGlobalBounds().height / 2.f < window->getSize().y) {
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
    //began after adding astroids

    this->projectile.rotate(angle);
    this->projectile.setPosition(this->character.getPosition());
    sf::Vector2i mousePos = sf::Mouse::getPosition(*(window));
    sf::Vector2f playerPos = this->character.getPosition();
    //find slope for projectile to follow
    float moveY = (mousePos.y - playerPos.y);
    float moveX = (mousePos.x - playerPos.x);
	sf::Vector2f velocity = sf::Vector2f(moveX, moveY);
    projectiles.push_back(Projectile{this->projectile, velocity});
}

void Player::updateProjectiles(sf::RenderWindow* window)
{

    /*

    delete out of range projectiles

    */
    for (size_t i = 0; i < projectiles.size(); i++) {
        if (projectiles[i].shape.getPosition().x < 0 || projectiles[i].shape.getPosition().x > window->getSize().x ||
            projectiles[i].shape.getPosition().y < 0 || projectiles[i].shape.getPosition().y > window->getSize().y)
        {
            projectiles.erase(projectiles.begin() + i);
            --i;
        }
        else
        {
            projectiles[i].shape.move(projectiles[i].velocity.x / 10, projectiles[i].velocity.y / 10);
            window->draw(projectiles[i].shape);
        }
    }
}

bool Player::checkCollision(std::list<Enemy> &enemies, std::vector<powerUp> &powerUp) {
    //check for collision between the projectiles and asteroids
    for (size_t i = 0; i < projectiles.size(); i++) {
        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (projectiles[i].shape.getGlobalBounds().intersects(it->get_bounds())) {
                projectiles.erase(projectiles.begin() + i);
                it = enemies.erase(it);
                break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
            }
        }
    }
    //checks for impact of player, asteroid and boost
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->get_bounds().intersects(character.getGlobalBounds())) {
            //create a lose function
            return true;
        }
    }
    for(size_t i = 0;powerUp.size() > i;i++)
    {
        if (powerUp[i].getBounds().intersects(character.getGlobalBounds()))
        {
            powerUp.erase(powerUp.begin() + i);
        }
    }
    return false;
}

//-----------------------------------------------enemy ------------------------------------------------------------------------

Enemy::Enemy()
{
    //load the texture
    if (!this->astroidTex.loadFromFile("textures/astroid.png")) {
        throw "could not load astroid.png";
    }

    //set asteroid texture
    this->astroid.setTexture(this->astroidTex);
    this->astroid.scale(0.5f, 0.5f);
}

Enemy::~Enemy()
{
}

void Enemy::createEnemies(const float& window_x, const float& window_y)
{
    /*
     *function uses equation of a circle by solving for y to spawn powerUps around the screen
     *y = k + sqrt(r^2 - (x-u)^2)
     */
    //generate random x cord
    float randomXSpawn = (rand() % 900);
    //get random cord and give it a random chance of being negative
    randomXSpawn =(rand() % 10) < 5 ? randomXSpawn * -1 : randomXSpawn;
    float randomYSpawn = 0;
    if(randomXSpawn > 0)
    {
		randomYSpawn = (window_x / 2.f) + std::sqrt(pow(window_y, 2) - pow((randomXSpawn - (window_y)),2));
    }else
    {
        //makes it possible to have a negative y cord if x is neg
        randomYSpawn = (window_x / 2.f) - std::sqrt(pow(window_y, 2) - pow(((randomXSpawn*-1.f) - (window_y)), 2));
        randomYSpawn *= -1.f;
    }

    std::cout << "x:" << randomXSpawn << " Y:" << randomYSpawn << std::endl;
    this->astroid.setPosition(sf::Vector2f(randomXSpawn, randomYSpawn));
}


sf::FloatRect Enemy::get_bounds()
{
    return this->astroid.getGlobalBounds();
}
sf::Vector2f Enemy::get_pos() 
{
    return this->astroid.getPosition();
}
void Enemy::draw(sf::RenderWindow* window)
{
    // TODO: why do i have to set texture hear
    astroid.setTexture(astroidTex);
    window->draw(astroid);
}
void Enemy::move(const float& x, const float& y)
{
    this->astroid.move(x, y);
}
//------------------------------------------------------power up -----------------------------------------------------
powerUp::powerUp()
{
    this->boostObj.setRadius(25);
    this->boostLevel = 1;
    //TODO: add effect and randomness of boosts and fix switch first
    /*switch (boostLevel) {
        case (1):
            this->boostObj.setFillColor(sf::Color::Yellow);
        case 2:
            this->boostObj.setFillColor(sf::Color::Cyan);
        case 3:
            this->boostObj.setFillColor(sf::Color::Green);
        default:
            throw "error in assigning color to the power up";
    }*/
}

powerUp::~powerUp()
{
}

void powerUp::CreatePowerUp(const float& window_x, const float& window_y)
{
    /*
     *function uses equation of a circle by solving for y to spawn powerUps around the screen
     *y = k + sqrt(r^2 - (x-u)^2)
     */
    
    //generate random x cord
    float randomXSpawn = (rand() % 900);
    //get random cord and give it a random chance of being negative
    randomXSpawn = (rand() % 10) < 5 ? randomXSpawn * -1 : randomXSpawn;
    float randomYSpawn = 0;
    if (randomXSpawn > 0)
    {
        randomYSpawn = (window_x / 2.f) + std::sqrt(pow(window_y, 2) - pow((randomXSpawn - (window_y)), 2));
    }
    else
    {
        //makes it possible to have a negative y cord if x is neg
        randomYSpawn = (window_x / 2.f) - std::sqrt(pow(window_y, 2) - pow(((randomXSpawn * -1.f) - (window_y)), 2));
        randomYSpawn *= -1.f;
    }
    this->boostObj.setPosition(sf::Vector2f(randomXSpawn, randomYSpawn));
}

void powerUp::move(const float& x, const float& y)
{
    this->boostObj.move(x,y);
}

void powerUp::draw(sf::RenderWindow* window)
{
	window->draw(this->boostObj);
}

sf::Vector2f powerUp::getPos()
{
    return this->boostObj.getPosition();
}

sf::FloatRect powerUp::getBounds()
{
    return this->boostObj.getGlobalBounds();
}


