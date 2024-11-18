#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Game::Game(): board(16, 25, 50), tileSize(32), isPaused(false) {
    window.create(sf::VideoMode(800, 600), "Minesweeper");

    loadResources();

    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setColor(sf::Color::Black);
    timerText.setPosition(10, 10);
}

void Game::loadResources() {
    if(!hiddenTexture.loadFromFile("files/images/tile_hidden.png") ||
       !revealedTexture.loadFromFile("files/images/tile_revealed.png")||
       !mineTexture.loadFromFile("files/images/mine.png")||
       !flagTexture.loadFromFile("files/images/flag.png")||
       !digitsTexture.loadFromFile("files/images/digits.png")||
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
}

void Game::run() {
    while(window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while(window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window.close();
        }
        else if(event.type == sf::Event::MouseButtonPressed) {
            int row = event.mouseButton.y / tileSize;
            int col = event.mouseButton.x / tileSize;
            if(event.mouseButton.button == sf::Mouse::Left) {
                board.revealTile(row, col);
            }
            else if(event.mouseButton.button == sf::Mouse::Right) {
                board.toggleFlag(row, col);
            }
        }
    }
}

void Game::drawNumber(int number, int xPosition, int yPosition, sf::RenderWindow& window) {
    ostringstream oss;
    oss << setw(3) << setfill('0') << number;
    std::string numberStr = oss.str();

    int digitWidth = 21;
    int digitHeight = 32;

    int offsetX = 0;

    if (number < 0) {
        sf::Sprite minusSprite;
        minusSprite.setTexture(digitsTexture);
        minusSprite.setTextureRect(sf::IntRect(10 * digitWidth, 0, digitWidth, digitHeight)); // 假设负号在第 10 位
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
    if(!isPaused) {
        sf::Time elapsed = gameClock.getElapsedTime();
        timerText.setString("Time: " + to_string(static_cast<int>(elapsed.asSeconds())));
    }

    if(board.checkVictory()) {
        cout << "Victory!" << endl;
        isPaused = true;
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

    int elapsedTime = static_cast<int>(gameClock.getElapsedTime().asSeconds());
    drawNumber(elapsedTime, 10, 550, window);

    int remainingMines = board.getRemainingMines();
    drawNumber(remainingMines, 200, 550, window);

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
//     sf::Text timerText;
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
