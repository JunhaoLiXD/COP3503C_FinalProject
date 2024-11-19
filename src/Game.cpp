#include "../include/Game.h"
#include "../include/WelcomeWindow.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Game::Game(): board(16, 25, 2), tileSize(32), isPaused(false), elapsedTime(0){
    minutesX = (board.getCol() * 32) - 97;
    secondsX = (board.getCol() * 32) - 54;
    timerY = (board.getRow() * 32) + 16;

    float buttonX = (board.getCol() * 32) / 2.0f - 16;
    float buttonY = (board.getRow() * 32) + 16;
    happyFaceButton.setPosition(buttonX, buttonY);
}

void Game::loadResources() {
    if(!hiddenTexture.loadFromFile("files/images/tile_hidden.png") ||
       !revealedTexture.loadFromFile("files/images/tile_revealed.png")||
       !mineTexture.loadFromFile("files/images/mine.png")||
       !flagTexture.loadFromFile("files/images/flag.png")||
       !digitsTexture.loadFromFile("files/images/digits.png")||
       !happyFaceTexture.loadFromFile("files/images/face_happy.png")||
       !loseFaceTexture.loadFromFile("files/images/face_lose.png")||
       !winFaceTexture.loadFromFile("files/images/face_win.png")||
       !font.loadFromFile("files/font.ttf")) {
        cerr << "Failed to load resources!" << endl;
        exit(1);
    }


    for(int i = 1; i < 8; i++) {
        sf::Texture texture;
        string filepath = "files/images/number_" + to_string(i) + ".png";
        if(!texture.loadFromFile(filepath)) {
            cerr << "Failed to load resources!" << endl;
            exit(1);
        }
        numberTextures.push_back(texture);
    }

    happyFaceButton.setTexture(happyFaceTexture);
}

void Game::run() {
    WelcomeWindow welcomWindow;
    string playerName = welcomWindow.run();

    if(!playerName.empty()) {
        cout << "Player name: " << playerName << endl;
    }
    else {
        cout << "Player does not exist!" << endl;
        return;
    }

    startTime = chrono::steady_clock::now();
    window.create(sf::VideoMode(800, 600), "Minesweeper");
    loadResources();

    while(window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::resetGame() {
    board.reset();
    isPaused = false;
    elapsedTime = 0;
    startTime = chrono::steady_clock::now();
    happyFaceButton.setTexture(happyFaceTexture);
}

void Game::handleEvents() {
    sf::Event event;
    while(window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window.close();
        }

        if(event.type == sf::Event::MouseButtonPressed) {
            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;

            if(happyFaceButton.getGlobalBounds().contains(mouseX, mouseY)) {
                resetGame();
                return;
            }

            if(isPaused) {
                continue;
            }

            int row = mouseY / tileSize;
            int col = mouseX / tileSize;

            if(event.mouseButton.button == sf::Mouse::Left) {
                board.revealTile(row, col);
            }
            else if(event.mouseButton.button == sf::Mouse::Right) {
                board.toggleFlag(row, col);
            }
        }
    }
}

void Game::drawNumber(int number, int xPosition, int yPosition, int digitCount, sf::RenderWindow& window) {
    ostringstream oss;
    oss << setw(digitCount) << setfill('0') << number;
    std::string numberStr = oss.str();

    int digitWidth = 21;
    int digitHeight = 32;

    int offsetX = 0;

    if (number < 0) {
        sf::Sprite minusSprite;
        minusSprite.setTexture(digitsTexture);
        minusSprite.setTextureRect(sf::IntRect(10 * digitWidth, 0, digitWidth, digitHeight));
        minusSprite.setPosition(xPosition + offsetX, yPosition);
        window.draw(minusSprite);
        offsetX += digitWidth;
    }

    for (int i = (number < 0 ? 1 : 0); i < numberStr.size(); i++) {
        int digit = numberStr[i] - '0';
        sf::Sprite digitSprite;
        digitSprite.setTexture(digitsTexture);
        digitSprite.setTextureRect(sf::IntRect(digit * digitWidth, 0, digitWidth, digitHeight));
        digitSprite.setPosition(xPosition + offsetX, yPosition);
        window.draw(digitSprite);
        offsetX += digitWidth;
    }
}

void Game::update() {
    if(isPaused) {
        return;
    }

    auto currentTime = chrono::steady_clock::now();
    elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

    if(board.checkVictory()) {
        isPaused = true;
        happyFaceButton.setTexture(winFaceTexture);
        return;
    }

    if(board.isGameOver()) {
        for(int row = 0; row < board.getRow(); row++) {
            for(int col = 0; col < board.getCol(); col++) {
                Tile& tile = board.getTile(row, col);
                if(tile.hasMine()) {
                    tile.reveal();
                }
            }
        }
        isPaused = true;
        happyFaceButton.setTexture(loseFaceTexture);
    }
}

void Game::render() {
    window.clear(sf::Color::White);

    for (int row = 0; row < board.getRow(); row++) {
        for (int col = 0; col < board.getCol(); col++) {
            sf::Sprite tileSprite;
            const Tile& tile = board.getTile(row, col);

            if (!tile.isTileRevealed()) {
                tileSprite.setTexture(hiddenTexture);
                tileSprite.setPosition(col * tileSize, row * tileSize);
                window.draw(tileSprite);

                if (tile.isTileFlagged()) {
                    sf::Sprite flagSprite;
                    flagSprite.setTexture(flagTexture);
                    flagSprite.setPosition(col * tileSize, row * tileSize);
                    window.draw(flagSprite);
                }
                if (board.isGameOver() && tile.hasMine()) {
                    sf::Sprite mineSprite;
                    mineSprite.setTexture(mineTexture);
                    mineSprite.setPosition(col * tileSize, row * tileSize);
                    window.draw(mineSprite);
                }
            }
            else {
                tileSprite.setTexture(revealedTexture);
                tileSprite.setPosition(col * tileSize, row * tileSize);
                window.draw(tileSprite);

                if (tile.hasMine()) {
                    sf::Sprite mineSprite;
                    mineSprite.setTexture(mineTexture);
                    mineSprite.setPosition(col * tileSize, row * tileSize);
                    window.draw(mineSprite);
                }
                else if (!tile.hasMine()) {
                    int adjacentMines = tile.getAdjacentMines();
                    if (adjacentMines > 0 && adjacentMines <= 8) {
                        sf::Sprite numberSprite;
                        numberSprite.setTexture(numberTextures[adjacentMines - 1]);
                        numberSprite.setPosition(col * tileSize, row * tileSize);
                        window.draw(numberSprite);
                    }
                }
            }
        }
    }

    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    drawNumber(minutes, minutesX, timerY, 2, window);
    drawNumber(seconds, secondsX, timerY, 2, window);

    int remainingMines = board.getRemainingMines();
    drawNumber(remainingMines, 33, timerY, 3, window);

    window.draw(happyFaceButton);
    window.display();
}


// class Game {
// private:
//     Board board;
//     int tileSize;
//     bool isPaused;
//     vector<sf::Texture> numberTextures;
//     sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture, digitsTexture;
//     sf::Font font;
//     sf::Clock gameClock;
//     sf::RenderWindow window;
//
//     void handleEvents();
//     void update();
//     void render();
//     void loadResources();
//
// public:
//     Game();
//     void run();
// };
