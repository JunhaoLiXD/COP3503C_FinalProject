#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <string>


class Game {
private:
    Board board;
    int tileSize;
    int minutesX;
    int secondsX;
    int timerY;
    int elapsedTime;
    bool isPaused;
    bool isVictory;
    bool isLeaderBoardOpen;
    bool isManuallyPaused;
    string playerName;
    string leaderboardFile = "files/leaderboard.txt";

    chrono::steady_clock::time_point startTime;
    chrono::steady_clock::time_point pauseStartTime;
    chrono::seconds pausedDuration;

    vector<sf::Texture> numberTextures;
    sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture, digitsTexture, happyFaceTexture,
                loseFaceTexture, winFaceTexture, leaderBoardTexture, pauseButtonTexture, playButtonTexture;
    sf::Font font;
    sf::RenderWindow window;
    sf::Sprite createDigitSprite(int digit, int xPosition, int yPosition);
    sf::Sprite happyFaceButton;
    sf::Sprite leaderBoardButton;
    sf::Sprite pausePlayButton;

    void handleEvents();
    void update();
    void render();
    void loadResources();
    void drawNumber(int number, int xPosition, int yPosition, int digitCount, sf::RenderWindow& window);
    void resetGame();
    void showLeaderBoard();
    void textToFile();



public:
    Game();
    void run();
};

#endif //GAME_H
