//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"

class Rook : public Piece{
public:
    Rook();
    Rook(bool color); //white : 1, black : 0
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //CHESS_ROOK_H
