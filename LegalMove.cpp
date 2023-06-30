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
            circles[tmp].setPosition((float) i * 187.5f+40, (float) j * 187.5f+40);
            circles[tmp].setRadius(50);
            circles[tmp].setFillColor(sf::Color(255, 255, 255, 0));
        }
    }
}

LegalMove::~LegalMove() = default;

void LegalMove::setMove(int row, int col)
{
    circles[col*8 + row].setFillColor(sf::Color(255, 255, 255, 100));
}

void LegalMove::reset() {
    for(int i = 0; i < 64; i++) {
        circles[i].setFillColor(sf::Color(255, 255, 255, 0));
    }
}

void LegalMove::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    for(int i = 0; i < 64; i++) {
        target.draw(circles[i]);
    }
}