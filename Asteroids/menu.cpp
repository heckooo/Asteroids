#include "menu.h"
#include "settings.h"
#include "Game.h"
#include <fstream>
#include <filesystem>
#include <algorithm>

Menu::Menu() {
    set_values();
}


void Menu::set_values() {
    window.create(sf::VideoMode(settings::window_width, settings::window_height), "Asteroids", sf::Style::Titlebar | sf::Style::Close);

    pos = 0;
    pressed = theselect = false;
    if (!font.loadFromFile("assets/ethn.otf")) throw std::runtime_error("Failed to load font");
    if (!image.loadFromFile("assets/menu.png")) throw std::runtime_error("Failed to load menu");

    bg.setTexture(image);

    pos_mouse = { 0,0 };
    mouse_coord = { 0, 0 };

    options = { "Asteroids", "Play", "Scores", "About", "Quit" };
    texts.resize(5);
    coords = { {590,40},{610,191},{590,282},{600,370},{623,457} };
    sizes = { 20,28,24,24,24 };

    for (std::size_t i{}; i < texts.size(); ++i) {
        texts[i].setFont(font);
        texts[i].setString(options[i]);
        texts[i].setCharacterSize(sizes[i]);
        texts[i].setOutlineColor(sf::Color::Black);
        texts[i].setPosition(coords[i]);
    }
    texts[1].setOutlineThickness(4);
    pos = 1;

    winclose.setSize(sf::Vector2f(23, 26));
    winclose.setPosition(1178, 39);
    winclose.setFillColor(sf::Color::Transparent);
}

std::vector<std::string> Menu::bestScores() {
    std::cout << "10 best scores:\n";
    std::ifstream file;
    std::string str;
    std::vector<int> scores;
    std::vector<std::string> bestScores;
    std::filesystem::path filePath = "scores.txt";
    if (std::filesystem::exists(filePath)) file.open("scores.txt");

    while (std::getline(file, str, ',')) {
        if (str.length() > 0) {
            scores.push_back(std::stoi(str));
        }
    }
    std::sort(scores.begin(), scores.end(), std::greater<>());

    for (int i = 1; i <= 10; i++) {
        std::string score = "";
        if (scores.size() >= i) {
            score = std::to_string(scores[i - 1]);
        }
        bestScores.push_back(std::to_string(i) + ". " + score);
        std::cout << std::to_string(i) + ". " + score << std::endl;
    }
    return bestScores;
}

void Menu::About() {
    std::cout << "Created by Patryk Hecko 2023 \n";
}

void Menu::loop_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        pos_mouse = sf::Mouse::getPosition(window);
        mouse_coord = window.mapPixelToCoords(pos_mouse);

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && !pressed) {
            if (pos < 4) {
                ++pos;
                pressed = true;
                texts[pos].setOutlineThickness(4);
                texts[pos - 1].setOutlineThickness(0);
                pressed = false;
                theselect = false;
            }
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && !pressed) {
            if (pos > 1) {
                --pos;
                pressed = true;
                texts[pos].setOutlineThickness(4);
                texts[pos + 1].setOutlineThickness(0);
                pressed = false;
                theselect = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !theselect) {
            theselect = true;
            if (pos == 1) {
                Game game(settings::window_width, settings::window_height, window);
                game.Initialize();
                game.Play();
            }

            if (pos == 2) {
                bestScores();
            }

            if (pos == 3) {
                About();
            }

            if (pos == 4) {
                window.close();
            }
            std::cout << options[pos] << '\n';
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (winclose.getGlobalBounds().contains(mouse_coord)) {
                //std::cout << "Close the window!" << '\n';
                window.close();
            }
        }
    }
}

void Menu::draw_all() {
    window.clear();
    window.draw(bg);
    for (auto t : texts) {
        window.draw(t);
    }
    window.display();
}

void Menu::run_menu() {
    while (window.isOpen()) {
        loop_events();
        draw_all();
    }
}