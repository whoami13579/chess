//
// Created by 褚翊喨 on 2023/6/30.
//

#include "Board.h"

Board::Board(sf::Color c1, sf::Color c2)
        : c1(c1), c2(c2) {
    chessBoard[0] = 'r';
    chessBoard[1] = 'n';
    chessBoard[2] = 'b';
    chessBoard[3] = 'q';
    chessBoard[4] = 'k';
    chessBoard[5] = 'b';
    chessBoard[6] = 'n';
    chessBoard[7] = 'r';
    for(int i = 8; i < 16; i++) {
        chessBoard[i] = 'p';
    }

    for(int i = 16; i < 48; i++) {
        chessBoard[i] = ' ';
    }

    for(int i = 48; i < 56; i++) {
        chessBoard[i] = 'P';
    }
    chessBoard[56] = 'R';
    chessBoard[57] = 'N';
    chessBoard[58] = 'B';
    chessBoard[59] = 'Q';
    chessBoard[60] = 'K';
    chessBoard[61] = 'B';
    chessBoard[62] = 'N';
    chessBoard[63] = 'R';

    int tmp;
    bool color = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tmp = (i * 8) + j;
            squares[tmp].setPosition((float) i * 187.5f, (float) j * 187.5f);
            squares[tmp].setSize(sf::Vector2f(187.5f, 187.5f));
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
    chessBoard[0] = 'r';
    chessBoard[1] = 'n';
    chessBoard[2] = 'b';
    chessBoard[3] = 'q';
    chessBoard[4] = 'k';
    chessBoard[5] = 'b';
    chessBoard[6] = 'n';
    chessBoard[7] = 'r';
    for(int i = 8; i < 16; i++) {
        chessBoard[i] = 'p';
    }

    for(int i = 16; i < 48; i++) {
        chessBoard[i] = ' ';
    }

    for(int i = 48; i < 56; i++) {
        chessBoard[i] = 'P';
    }
    chessBoard[56] = 'R';
    chessBoard[57] = 'N';
    chessBoard[58] = 'B';
    chessBoard[59] = 'Q';
    chessBoard[60] = 'K';
    chessBoard[61] = 'B';
    chessBoard[62] = 'N';
    chessBoard[63] = 'R';

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