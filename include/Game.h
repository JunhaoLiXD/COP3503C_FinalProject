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
    int rows;
    int cols;
    int mines;
    int tileSize;
    int minutesX;
    int secondsX;
    int timerY;
    int elapsedTime;
    bool isPaused;
    bool isVictory;
    bool isLeaderBoardOpen;
    bool isManuallyPaused;
    bool isDebugMode;
    bool isNewRecord;
    string playerName;
    string leaderboardFile = "files/leaderboard.txt";

    chrono::steady_clock::time_point startTime;
    chrono::steady_clock::time_point pauseStartTime;
    chrono::seconds pausedDuration;

    vector<sf::Texture> numberTextures;
    sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture, digitsTexture, happyFaceTexture,
                loseFaceTexture, winFaceTexture, leaderBoardTexture, pauseButtonTexture, playButtonTexture,
                debugButtonTexture;
    sf::Font font;
    sf::RenderWindow window;
    sf::RenderWindow leaderboardWindow;
    sf::Sprite happyFaceButton;
    sf::Sprite leaderBoardButton;
    sf::Sprite pausePlayButton;
    sf::Sprite debugButton;

    void handleEvents();
    void update();
    void render();
    void loadResources();
    void drawNumber(int number, int xPosition, int yPosition, int digitCount, sf::RenderWindow& window);
    void resetGame();
    void showLeaderBoard();
    void textToFile();
    void handleLeaderboardWindowEvents();
    void loadConfig(const string& configPath);

public:
    Game();
    void run();
};

#endif //GAME_H
