#include "ChessGame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    ChessGame chessGame(sf::Color(0xf3bc7aff), sf::Color(0xae722bff));

    sf::RenderWindow window(sf::VideoMode(2000, 1500), "chess", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::Event event = sf::Event();

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if ((0 < event.mouseButton.x) && (event.mouseButton.x < 1500) && (0 < event.mouseButton.y) &&
                        (event.mouseButton.y < 1500)) {
                        chessGame.test(event, window, int (event.mouseButton.y/187.5), int (event.mouseButton.x/187.5));
                    } else if ((1500 < event.mouseButton.x) && (event.mouseButton.x < 2000) &&
                               (0 < event.mouseButton.y) && (event.mouseButton.y < 50)) {
                        chessGame.restart();
                    }
                }
            }
        }
        window.draw(chessGame);
        window.display();
    }

    return 0;
}
