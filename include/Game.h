#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>
#include <vector>


class Game {
private:
    Board board;
    int tileSize;
    bool isPaused;
    vector<sf::Texture> numberTextures;
    sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture, digitsTexture;
    sf::Font font;
    sf::Text timerText;
    sf::Clock gameClock;
    sf::RenderWindow window;

    void handleEvents();
    void update();
    void render();
    void loadResources();
    sf::Sprite createDigitSprite(int digit, int xPosition, int yPosition);
    void drawNumber(int number, int xPosition, int yPosition, sf::RenderWindow& window);


public:
    Game();
    void run();
};

#endif //GAME_H
