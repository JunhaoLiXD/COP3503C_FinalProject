#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>


class Game {
private:
    sf::RenderWindow window;
    Board board;
    int tileSize;
    sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture;
    sf::Font font;
    sf::Text timerText;
    bool isPaused;
    sf::Clock gameClock;

    void handleEvents();
    void update();
    void render();
    void loadResources();

public:
    Game();
    void run();
};

#endif //GAME_H
