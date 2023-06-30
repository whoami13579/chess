//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H


#include "Piece.h"

class Bishop : public Piece{
public:
    Bishop();

    Bishop(bool color); // white : 1, black : 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
};


#endif //CHESS_BISHOP_H
