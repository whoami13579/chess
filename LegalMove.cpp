//
// Created by 褚翊喨 on 2023/6/30.
//

#include "LegalMove.h"

LegalMove::LegalMove()
{
    int tmp;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tmp = (i * 8) + j;
            circles[tmp].setPosition((float) i * 80+15, (float) j * 80+15);
            circles[tmp].setRadius(25);
            circles[tmp].setFillColor(sf::Color(255, 255, 255, 0));
        }
    }

    for(int i = 0; i < 64; i++) {
        bitBoard[i] = false;
    }
}

LegalMove::~LegalMove() = default;

void LegalMove::setMove(int row, int col)
{
    circles[col*8 + row].setFillColor(sf::Color(255, 255, 255, 100));
    bitBoard[row*8 + col] = true;
}

void LegalMove::reset() {
    for(int i = 0; i < 64; i++) {
        circles[i].setFillColor(sf::Color(255, 255, 255, 0));
    }

    for(int i = 0; i < 64; i++) {
        bitBoard[i] = false;
    }
}

void LegalMove::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    for(int i = 0; i < 64; i++) {
        target.draw(circles[i]);
    }
}