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

        //TODO: why does it not like to shoot projectiles right when enemies spawn
        /*
         * to recreate hold down the fire button while turning and moving
         * The projectiles are moving but they are behind or not update with the player
         */

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

    if(frameCounter >= 80) {
        enemies.push_back(Enemy());
        enemies.back().createEnemies(window->getSize().x,window->getSize().y);
        frameCounter = 0;
    }
    for (size_t i = 0; i < enemies.size(); i++) {
        //get location
        const sf::Vector2f location = this->enemies[i].get_pos();

        if (location.x - 901 > size.x - location.x) {
            enemies.erase(enemies.begin() + i);
            continue;
        }else if (location.y - 901 > size.y - location.y) {
            enemies.erase(enemies.begin() + i);
            continue;
        }else if (location.x + 901 < 0 - location.x) {
            enemies.erase(enemies.begin() + i);
            continue;
        }else if (location.y + 901 < 0 - location.y) {
            enemies.erase(enemies.begin() + i);
            continue;
        }
        //find the slope

        float slopeY = size.y / 2.f - location.y;
        float slopeX = size.x / 2.f - location.x;
        float slope = slopeY / slopeX;

        //find next point on line, point slope form
        //curent location 1 is the x cord, loaction 0 is y
        float nextY = -1 * (slope * ((location.x - 4.f) - location.x)) + location.y;

        enemies[i].move(4.f, (nextY - location.y));
        enemies[i].draw(this->window);
    }

}

void Game::make_powerUp(sf::Vector2u size)
{
    ++powerUpFrameCounter;

    if (powerUpFrameCounter >= 800) {
        powerUps.push_back(powerUp());
        powerUps.back().CreatePowerUp();
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
        else if (location.x + 901 < 0 - location.x) {
            powerUps.erase(powerUps.begin() + i);
            continue;
        }
        else if (location.y + 901 < 0 - location.y) {
            powerUps.erase(powerUps.begin() + i);
            continue;
        }
        //find the slope

        float slopeY = size.y / 2.f - location.y;
        float slopeX = size.x / 2.f - location.x;
        float slope = slopeY / slopeX;

        //find next point on line, point slope form
        //curent location 1 is the x cord, loaction 0 is y
        float nextY = -1 * (slope * ((location.x - 4.f) - location.x)) + location.y;

        powerUps[i].move(4.f, (nextY - location.y));
        powerUps[i].draw(this->window);
    }

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
        } else if (projectieles[i].getPosition().y < 0 || projectieles[i].getPosition().y > window->getSize().y) {
            projectieles.erase(projectieles.begin() + i);
            projectielePathX.erase(projectielePathX.begin() + i);
            projectielePathY.erase(projectielePathY.begin() + i);
            continue;
        } else {
            projectieles[i].move(projectielePathX[i] / 10, projectielePathY[i] / 10);
            window->draw(projectieles[i]);
        }
    }
}

bool Player::checkCollision(std::vector<Enemy> &enemies) {
    //check for colllision between the projectiles and astroids
    for (size_t i = 0; i < projectieles.size(); i++) {
        for (size_t j = 0; enemies.size() > j; j++) {
            if (projectieles[i].getGlobalBounds().intersects(enemies[j].get_bounds())) {
                projectieles.erase(projectieles.begin() + i);
                enemies.erase(enemies.begin() + j);
                break;//we have to break out bc it will check a projectile that no longer igists if we stay in loop
            }
        }
    }
    //checks for inpact of player and astroid
    for (size_t i = 0; enemies.size() > i; i++) {
        if (enemies[i].get_bounds().intersects(character.getGlobalBounds())) {
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
    if (!this->astroidTex.loadFromFile("textures/astroid.png")) {
        throw "could not load astroid.png";
    }

    //set astroid texture
    this->astroid.setTexture(this->astroidTex);
    this->astroid.scale(0.5f, 0.5f);
}

Enemy::~Enemy()
{
}

void Enemy::createEnemies(const float& window_x, const float& window_y)
{
    // TODO: fix spawn alguritem enemy
    //y = k + sqrt(r^2 - (x-u)^2)
    //y = k- sqrt(r^2 - (x-u)^2)  
    double randomXSpawn = (rand() % 900) * -1.f;
    double randomYSpawn = (window_x / 2) + std::sqrt(pow(900, 2) - pow((randomXSpawn - (window_y / 2)),2));
    //float randomXSpawn = (rand() % 900) * -1.f;
    //float randomYSpawn = (rand() % 900) * -1.f;
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
    //TODO: why do i have to set texture hear
    this->astroid.setTexture(this->astroidTex);
    window->draw(this->astroid);
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
    //TODO: add effect and randomness of boosts
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

void powerUp::CreatePowerUp()
{
    // TODO: fix spawn algorithm power up

    float randomSpawnX = (rand() % 900) * -1.f;
    float randomSpawnY = (rand() % 900) * -1.f;
    // float randomSpawnX = 500;
    // float randomSpawnY = 500;
    this->boostObj.setPosition(sf::Vector2f(randomSpawnX, randomSpawnY));
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


