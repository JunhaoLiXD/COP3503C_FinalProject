#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>


class Game {
private:
    Board board;
    int tileSize;
    int minutesX;
    int secondsX;
    int timerY;
    int elapsedTime;
    bool isPaused;
    chrono::steady_clock::time_point startTime;

    vector<sf::Texture> numberTextures;
    sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture, digitsTexture, happyFaceTexture,
                loseFaceTexture, winFaceTexture;
    sf::Font font;
    sf::RenderWindow window;
    sf::Sprite createDigitSprite(int digit, int xPosition, int yPosition);
    sf::Sprite happyFaceButton;

    void handleEvents();
    void update();
    void render();
    void loadResources();
    void drawNumber(int number, int xPosition, int yPosition, int digitCount, sf::RenderWindow& window);
    void resetGame();


public:
    Game();
    void run();
};

#endif //GAME_H
