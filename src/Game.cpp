#include "../include/Game.h"
#include "../include/WelcomeWindow.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

Game::Game() : tileSize(32), isPaused(false), isVictory(false), isManuallyPaused(false), isDebugMode(false),
                isLeaderBoardOpen(false), isNewRecord(false), elapsedTime(0), pausedDuration(chrono::seconds(0)){

    loadConfig("files/config.cfg");

    board.initializeBoard(rows, cols, mines);

    minutesX = (board.getCol() * 32) - 97;
    secondsX = (board.getCol() * 32) - 54;
    timerY = (board.getRow() + 0.5) * 32 + 16;

    float buttonX = (board.getCol() * 32) / 2.0f - 32;
    float buttonY = (board.getRow()+0.5) * 32;
    happyFaceButton.setPosition(buttonX, buttonY);

    float leaderBoardButtonX = (board.getCol() * 32) - 176;
    float leaderBoardButtonY = (board.getRow()+0.5) * 32;
    leaderBoardButton.setPosition(leaderBoardButtonX, leaderBoardButtonY);

    float pausePlayButtonX = (board.getCol() * 32) - 240;
    float pausePlayButtonY = (board.getRow()+0.5) * 32;
    pausePlayButton.setPosition(pausePlayButtonX, pausePlayButtonY);

    float debugButtonX = (board.getCol() * 32) - 304;
    float debugButtonY = (board.getRow()+0.5) * 32;
    debugButton.setPosition(debugButtonX, debugButtonY);
}

void Game::loadConfig(const string& configPath) {
    std::ifstream configFile(configPath);

    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open config file: " + configPath);
    }

    configFile >> cols >> rows >> mines;

    if (cols < 22 || rows < 16 || mines < 0 || mines > cols * rows) {
        throw std::runtime_error("Invalid configuration values in config file");
    }

    configFile.close();
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
       !leaderBoardTexture.loadFromFile("files/images/leaderboard.png")||
       !pauseButtonTexture.loadFromFile("files/images/pause.png")||
       !playButtonTexture.loadFromFile("files/images/play.png")||
       !debugButtonTexture.loadFromFile("files/images/debug.png")||
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
    leaderBoardButton.setTexture(leaderBoardTexture);
    pausePlayButton.setTexture(pauseButtonTexture);
    debugButton.setTexture(debugButtonTexture);
}

void Game::run() {
    WelcomeWindow welcomWindow;
    playerName = welcomWindow.run();

    startTime = chrono::steady_clock::now();
    window.create(sf::VideoMode(800, 600), "Minesweeper");
    loadResources();

    while(window.isOpen()) {
        handleEvents();
        handleLeaderboardWindowEvents();
        update();
        render();
    }
}

void Game::resetGame() {
    board.reset();
    isPaused = false;
    isManuallyPaused = false;
    isVictory = false;
    isNewRecord = false;
    elapsedTime = 0;
    startTime = chrono::steady_clock::now();
    pausedDuration = chrono::seconds(0);
    happyFaceButton.setTexture(happyFaceTexture);
    pausePlayButton.setTexture(pauseButtonTexture);
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

            if (pausePlayButton.getGlobalBounds().contains(mouseX, mouseY)) {
                if (!isVictory && !board.isGameOver()) {
                    isManuallyPaused = !isManuallyPaused;
                    if (isManuallyPaused) {
                        pausePlayButton.setTexture(playButtonTexture);
                        pauseStartTime = chrono::steady_clock::now();
                    }
                    else {
                        pausePlayButton.setTexture(pauseButtonTexture);
                        pausedDuration += chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - pauseStartTime);
                    }
                }
                return;
            }

            if (debugButton.getGlobalBounds().contains(mouseX, mouseY)) {
                if (!isVictory && !board.isGameOver()) {
                    isDebugMode = !isDebugMode;
                }
                return;
            }

            if(happyFaceButton.getGlobalBounds().contains(mouseX, mouseY)) {
                resetGame();
                return;
            }

            if (leaderBoardButton.getGlobalBounds().contains(mouseX, mouseY)) {
                if (isLeaderBoardOpen) {
                    leaderboardWindow.close();
                    isLeaderBoardOpen = false;
                    if(!isVictory && !board.isGameOver()) {
                        isPaused = false;
                        pausedDuration += chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - pauseStartTime);
                    }
                } else {
                    leaderboardWindow.create(sf::VideoMode(300, 400), "Minesweeper Leaderboard");
                    isLeaderBoardOpen = true;
                    showLeaderBoard();
                }
                return;
            }

            if(isPaused || isManuallyPaused) {
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

void Game::showLeaderBoard() {
    isPaused = true;
    pauseStartTime = chrono::steady_clock::now();

    font.loadFromFile("files/font.ttf");
    vector<pair<string, string>> leaderboard;
    ifstream infile(leaderboardFile);
    string line;

    while (getline(infile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string time = line.substr(0, commaPos);
            string name = line.substr(commaPos + 1);
            leaderboard.emplace_back(time, name);
        }
    }
    infile.close();
    sort(leaderboard.begin(), leaderboard.end());

    vector<sf::Text> entries;
    for (int i = 0; i < leaderboard.size() && i < 5; i++) {
        ostringstream formattedEntry;
        if(leaderboard[i].second == playerName && isNewRecord) {
            formattedEntry << i + 1 << ". " << leaderboard[i].first << " " << leaderboard[i].second << "*";
        }
        else {
            formattedEntry << i + 1 << ". " << leaderboard[i].first << " " << leaderboard[i].second;
        }
        sf::Text text(formattedEntry.str(), font, 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(40, 80 + i * 40);
        entries.push_back(text);
    }

    sf::Text title("LEADERBOARD", font, 30);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setPosition(60, 20);

    sf::RectangleShape background(sf::Vector2f(300, 400));
    background.setFillColor(sf::Color(0, 0, 255));

    leaderboardWindow.clear();
    leaderboardWindow.draw(background);
    leaderboardWindow.draw(title);
    for (const auto& entry : entries) {
        leaderboardWindow.draw(entry);
    }
    leaderboardWindow.display();
}

void Game::handleLeaderboardWindowEvents() {
    if (isLeaderBoardOpen) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboardWindow.close();
                isLeaderBoardOpen = false;

                if (!isVictory && !board.isGameOver()) {
                    isPaused = false;
                    pausedDuration += chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - pauseStartTime);
                }
            }
        }
    }
}

void Game::textToFile() {
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    ostringstream formattedTime;
    formattedTime << setw(2) << setfill('0') << minutes << ":"
                  << setw(2) << setfill('0') << seconds;

    vector<pair<string, string>> leaderboard;

    ifstream infile(leaderboardFile);
    string line;
    if (infile.is_open()) {
        while (getline(infile, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string time = line.substr(0, commaPos);
                string name = line.substr(commaPos + 1);
                leaderboard.emplace_back(time, name);
            }
        }
        infile.close();
    } else {
        cerr << "Failed to open leaderboard file for reading!" << endl;
    }

    bool updated = false;
    for (auto& entry : leaderboard) {
        if (entry.second == playerName) {
            if (formattedTime.str() < entry.first) {
                entry.first = formattedTime.str();
                isNewRecord = true;
            }
            updated = true;
            break;
        }
    }

    if (!updated) {
        leaderboard.emplace_back(formattedTime.str(), playerName);
        isNewRecord = true;
    }

    sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, string>& a, const pair<string, string>& b) {
        return a.first < b.first;
    });

    ofstream outfile(leaderboardFile);
    if (outfile.is_open()) {
        int count = 0;
        for (const auto& entry : leaderboard) {
            if (count++ >= 5) break;
            outfile << entry.first << "," << entry.second << endl;
        }
        outfile.close();
    } else {
        cerr << "Failed to open leaderboard file for writing!" << endl;
    }
}


void Game::update() {
    if(isVictory) {
        return;
    }

    if(isPaused || isManuallyPaused) {
        return;
    }

    auto currentTime = chrono::steady_clock::now();
    elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime - pausedDuration).count();


    if(board.checkVictory()) {
        isPaused = true;
        isVictory = true;
        happyFaceButton.setTexture(winFaceTexture);
        leaderboardWindow.create(sf::VideoMode(300, 400), "Minesweeper Leaderboard");
        isLeaderBoardOpen = true;
        textToFile();
        showLeaderBoard();
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

            if (isManuallyPaused) {
                tileSprite.setTexture(revealedTexture);
                tileSprite.setPosition(col * tileSize, row * tileSize);
                window.draw(tileSprite);
            }
            else if (!tile.isTileRevealed()) {
                tileSprite.setTexture(hiddenTexture);
                tileSprite.setPosition(col * tileSize, row * tileSize);
                window.draw(tileSprite);

                if (tile.isTileFlagged()) {
                    sf::Sprite flagSprite;
                    flagSprite.setTexture(flagTexture);
                    flagSprite.setPosition(col * tileSize, row * tileSize);
                    window.draw(flagSprite);
                }
                if ((board.isGameOver() || isDebugMode) && tile.hasMine()) {
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
    window.draw(leaderBoardButton);
    window.draw(pausePlayButton);
    window.draw(debugButton);
    window.display();
}
