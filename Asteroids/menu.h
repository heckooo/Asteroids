#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Menu {
    int pos;
    bool pressed, theselect;

    sf::RenderWindow window;
    sf::RectangleShape winclose;
    sf::Font font;
    sf::Texture image;
    sf::Sprite bg;

    sf::Vector2i pos_mouse;
    sf::Vector2f mouse_coord;

    std::vector<const char*> options;
    std::vector<sf::Vector2f> coords;
    std::vector<sf::Text> texts;
    std::vector<std::size_t> sizes;

protected:
    void set_values();
    void loop_events();
    void draw_all();

public:
    Menu();
    void run_menu();
    std::vector<std::string> bestScores();
    void About();
};
