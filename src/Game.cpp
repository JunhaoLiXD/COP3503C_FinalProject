#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

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
       !font.loadFromFile("files/font.ttf")) {
        cerr << "Failed to load resources!" << endl;
        exit(1);
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

    for(int row = 0; row < board.getRow(); row++) {
        for(int col = 0; col < board.getCol(); col++) {
            sf::Sprite tileSprite;
            if(board.getTile(row, col).isTileRevealed()) {
                if(board.getTile(row, col).hasMine()) {
                    tileSprite.setTexture(mineTexture);
                }
                else {
                    tileSprite.setTexture(revealedTexture);
                }
            }
            else {
                if(board.getTile(row, col).isTileFlagged()) {
                    tileSprite.setTexture(flagTexture);
                }
                else {
                    tileSprite.setTexture(hiddenTexture);
                }
            }
            tileSprite.setPosition(col * tileSize, row * tileSize);
            window.draw(tileSprite);
        }
    }

    window.draw(timerText);
    window.display();
}


// class Game {
// private:
//     sf::RenderWindow* window;
//     Board board;
//     int tileSize;
//     sf::Texture hiddenTexture, revealedTexture, mineTexture, flagTexture;
//     sf::Font font;
//     sf::Text timerText;
//     bool isPaused;
//     sf::Clock gameClock;
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
