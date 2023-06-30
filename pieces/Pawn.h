//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H


#include "Piece.h"

class Pawn : public Piece{
public:
    Pawn();
    Pawn(bool color); // white : 1, black : 0
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

};


#endif //CHESS_PAWN_H
