#include "ChessGame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main() {
    ChessGame chessGame(sf::Color(0xf3bc7aff), sf::Color(0xae722bff));

    sf::RenderWindow window(sf::VideoMode(1700, 1200), "chess", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::Event event = sf::Event();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if ((0 < event.mouseButton.x) && (event.mouseButton.x < 1200) && (0 < event.mouseButton.y) &&
                        (event.mouseButton.y < 1200)) {
                        chessGame.select(event, window, int(event.mouseButton.y / 150), int(event.mouseButton.x / 150));
                    } else if ((1200 < event.mouseButton.x) && (event.mouseButton.x < 1700) &&
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
