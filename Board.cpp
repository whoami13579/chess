//
// Created by 褚翊喨 on 2023/6/30.
//

#include "Board.h"

Board::Board(sf::Color c1, sf::Color c2)
        : c1(c1), c2(c2) {
    bitBoard[0] = 'r';
    bitBoard[1] = 'n';
    bitBoard[2] = 'b';
    bitBoard[3] = 'q';
    bitBoard[4] = 'k';
    bitBoard[5] = 'b';
    bitBoard[6] = 'n';
    bitBoard[7] = 'r';
    for(int i = 8; i < 16; i++) {
        bitBoard[i] = 'p';
    }

    for(int i = 16; i < 48; i++) {
        bitBoard[i] = ' ';
    }

    for(int i = 48; i < 56; i++) {
        bitBoard[i] = 'P';
    }
    bitBoard[56] = 'R';
    bitBoard[57] = 'N';
    bitBoard[58] = 'B';
    bitBoard[59] = 'Q';
    bitBoard[60] = 'K';
    bitBoard[61] = 'B';
    bitBoard[62] = 'N';
    bitBoard[63] = 'R';

    int tmp;
    bool color = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tmp = (i * 8) + j;
            squares[tmp].setPosition(i * 150, j * 150);
            squares[tmp].setSize(sf::Vector2f((float)150, (float)150));
            squares[tmp].setFillColor((color ? c1 : c2));

            color = !color;
        }
        color = !color;
    }
}

void Board::setColor(int row, int col) {
    squares[col * 8 + row].setFillColor(sf::Color::Green);
}

void Board::reset() {
    bitBoard[0] = 'r';
    bitBoard[1] = 'n';
    bitBoard[2] = 'b';
    bitBoard[3] = 'q';
    bitBoard[4] = 'k';
    bitBoard[5] = 'b';
    bitBoard[6] = 'n';
    bitBoard[7] = 'r';
    for(int i = 8; i < 16; i++) {
        bitBoard[i] = 'p';
    }

    for(int i = 16; i < 48; i++) {
        bitBoard[i] = ' ';
    }

    for(int i = 48; i < 56; i++) {
        bitBoard[i] = 'P';
    }
    bitBoard[56] = 'R';
    bitBoard[57] = 'N';
    bitBoard[58] = 'B';
    bitBoard[59] = 'Q';
    bitBoard[60] = 'K';
    bitBoard[61] = 'B';
    bitBoard[62] = 'N';
    bitBoard[63] = 'R';

    bool color = true;
    for (int i = 0; i < 64; i++) {
        squares[i].setFillColor(color ? c1 : c2);
        color = !color;

        if (i % 8 == 0)
            color = !color;
    }
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (int i = 0; i < 64; i++) {
        target.draw(squares[i]);
    }
}