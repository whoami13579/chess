//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H


#include "Piece.h"

class Queen : public Piece{
public:
    Queen();

    Queen(bool color); // white : 1, black : 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif //CHESS_QUEEN_H
