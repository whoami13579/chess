#include "ChessGame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(890, 640), "chess", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::Event event = sf::Event();

    ChessGame chessGame(&window, &event, sf::Color(0xf3bc7aff), sf::Color(0xae722bff));
    int x, y; // the position of mouse click

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }


            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    x = event.mouseButton.x;
                    y = event.mouseButton.y;
                    if ((0 < x) && (x < 640) && (0 < y) && (y < 640)) {
                        chessGame.select(int(y / 80), int(x / 80));
                    } else if ((640 < x) && (x < 890) && (0 < y) && (y < 50)) {
                        chessGame.restart();
                    }
                }
            }
        }
        window.clear();
        window.draw(chessGame);
        window.display();
    }

    return 0;
}
