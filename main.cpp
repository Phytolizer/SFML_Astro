#include "Game.h"

int main() {
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
	
//pick up note fix math for astroids movement 