//
// Created by 褚翊喨 on 2023/6/30.
//

#include "Piece.h"

Piece::Piece() {}

void Piece::setPosition(int row, int col)
{
    positionY = row;
    positionX = col;

    sprite.setPosition(sf::Vector2f(positionX*187.5 + 187.5/2, positionY*187.5 + 187.5/2));
}

int Piece::getPositionX()
{
    return positionX;
}

int Piece::getPositionY()
{
    return positionY;
}