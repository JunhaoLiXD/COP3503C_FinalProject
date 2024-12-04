#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;

class WelcomeWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text inputText;
    sf::Text cursorText;
    sf::Text welcomeText;
    sf::Text enterNameText;
    string userInput;
    string cursor;

    void handleEvents();
    void render();

public:
    WelcomeWindow();
    string run();
};

#endif
