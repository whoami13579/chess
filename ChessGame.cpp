//
// Created by 褚翊喨 on 2023/6/30.
//

#include "ChessGame.h"

ChessGame::ChessGame(sf::Color c1, sf::Color c2) {
    board = Board(c1, c2);

//    font.loadFromFile("/Users/mba/myCode/chess/Textures/arial.ttf");
    font.loadFromFile("../Textures/arial.ttf");

    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(-5.f);
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setPosition(sf::Vector2f(1500.f, 0.f));
    restartButton.setSize(sf::Vector2f(500.f, 50.f));

    restartText.setFont(font);
    restartText.setString("RESTART");
    restartText.setCharacterSize(24);
    restartText.setStyle(sf::Text::Bold);
    restartText.setFillColor(sf::Color::Black);
    restartText.setPosition(restartButton.getPosition().x + 180.f, restartButton.getPosition().y + 10.f);

    createMaterials();
    restart();
}

void ChessGame::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(board);
//    target.draw(pieces);
    drawMaterial(target, states);
    target.draw(legalMove);
    target.draw(restartButton);
    target.draw(restartText);
}

void ChessGame::restart() {
    board.reset();
    legalMove.reset();
    int tmp;
    bool color = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tmp = (i * 8) + j;
            board.squares[tmp].setPosition((float) i * 187.5f, (float) j * 187.5f);
            board.squares[tmp].setSize(sf::Vector2f(187.5f, 187.5f));
            board.squares[tmp].setFillColor((color ? board.c1 : board.c2));

            color = !color;
        }
        color = !color;
    }


    materials[0].setPosition(7, 4);
    materials[1].setPosition(0, 4);
    materials[2].setPosition(7, 3);
    materials[3].setPosition(0, 3);
    materials[4].setPosition(7, 2);
    materials[5].setPosition(7, 5);
    materials[6].setPosition(0, 2);
    materials[7].setPosition(0, 5);
    materials[8].setPosition(7, 1);
    materials[9].setPosition(7, 6);
    materials[10].setPosition(0, 1);
    materials[11].setPosition(0, 6);
    materials[12].setPosition(7, 0);
    materials[13].setPosition(7, 7);
    materials[14].setPosition(0, 0);
    materials[15].setPosition(0, 7);
    for(int i = 0; i < 8; i++) {
        materials[i+16].setPosition(6, i);
    }
    for(int i = 0; i < 8; i++) {
        materials[i+24].setPosition(1, i);
    }
}

void ChessGame::test(sf::Event &event, sf::RenderWindow &window, int row, int col) {
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if ((0 < event.mouseButton.x) && (event.mouseButton.x < 1500) && (0 < event.mouseButton.y) &&
                        (event.mouseButton.y < 1500)) {
                        legalMove.setMove(row, col);
                        return;
                    } else if ((1500 < event.mouseButton.x) && (event.mouseButton.x < 2000) &&
                               (0 < event.mouseButton.y) && (event.mouseButton.y < 50)) {
                        restart();
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    return;
                }
            }
        }
    }
}

void ChessGame::drawMaterial(sf::RenderTarget &target, sf::RenderStates states) const {
    for(int i = 0; i < 32; i++) {
        materials[i].draw(target, states);
    }
}

void ChessGame::createMaterials() {
    materials[0] = Material('K');
    materials[1] = Material('k');
    materials[2] = Material('Q');
    materials[3] = Material('q');
    materials[4] = Material('B');
    materials[5] = Material('B');
    materials[6] = Material('b');
    materials[7] = Material('b');
    materials[8] = Material('N');
    materials[9] = Material('N');
    materials[10] = Material('n');
    materials[11] = Material('n');
    materials[12] = Material('R');
    materials[13] = Material('R');
    materials[14] = Material('r');
    materials[15] = Material('r');

    for(int i = 16; i < 24; i++) {
        materials[i] = Material('P');
    }
    for(int i = 24; i < 32; i++) {
        materials[i] = Material('p');
    }
}