#include "../include/WelcomeWindow.h"

#include <iostream>
#include <ostream>
#include <string>
#include <SFML/Graphics.hpp>

WelcomeWindow::WelcomeWindow() : window(sf::VideoMode(800, 600), "Welcome"), userInput(""), cursor("|") {
    if(!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        exit(1);
    }

    // Init input text
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setPosition(800 / 2 - welcomeText.getGlobalBounds().width / 2,
                            600 / 2 - welcomeText.getGlobalBounds().height / 2 - 45);

    // Init cursor
    cursorText.setFont(font);
    cursorText.setCharacterSize(18);
    cursorText.setFillColor(sf::Color::Yellow);

    //Init welcome text
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setPosition(800 / 2 - welcomeText.getGlobalBounds().width / 2,
                            600 / 2 - welcomeText.getGlobalBounds().height / 2 - 150);

    //Init enterName text
    enterNameText.setFont(font);
    enterNameText.setString("Enter your name:");
    enterNameText.setCharacterSize(20);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setStyle(sf::Text::Bold);
    enterNameText.setPosition(800 / 2 - enterNameText.getGlobalBounds().width / 2,
                                600 / 2 - welcomeText.getGlobalBounds().height / 2 - 75);
}

string WelcomeWindow::run() {
    while(window.isOpen()) {
        handleEvents();
        render();
    }
    window.close();
    return userInput;
}

void WelcomeWindow::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            exit(1);
        }
        else if (event.type == sf::Event::TextEntered) {
            if (userInput.size() < 10 && std::isalpha(event.text.unicode)) {
                userInput += static_cast<char>(event.text.unicode);
                if (!userInput.empty()) {
                    userInput[0] = std::toupper(userInput[0]);
                    for (size_t i = 1; i < userInput.size(); ++i) {
                        userInput[i] = std::tolower(userInput[i]);
                    }
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                if (!userInput.empty()) {
                    window.close();
                }
            }
            else if (event.key.code == sf::Keyboard::Backspace) {
                if (!userInput.empty()) {
                    userInput.pop_back();
                }
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}


void WelcomeWindow::render() {
    window.clear(sf::Color::Blue);

    window.draw(welcomeText);
    window.draw(enterNameText);

    inputText.setString(userInput);
    cursorText.setString(cursor);

    float cursorX = inputText.getPosition().x + inputText.getGlobalBounds().width;
    cursorText.setPosition(cursorX, inputText.getPosition().y);

    window.draw(inputText);
    window.draw(cursorText);

    window.display();
}

