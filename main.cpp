#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(nullptr));
    Game game;
    //the window loop that keeps it open and running
    while (game.getWindowIsOpen())
    {
        //update
        game.update();

        //draw
        game.render();

    }
    return 0;
}
	
