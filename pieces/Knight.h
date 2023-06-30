//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H


#include "Piece.h"

class Knight : public Piece{
public:
    Knight();

    Knight(bool color); // white : 1, black : 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
};


#endif //CHESS_KNIGHT_H
