//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_KING_H
#define CHESS_KING_H


#include "Piece.h"

class King : public Piece {
public:
    King();

    King(bool color); // white : 1, black : 0

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
};


#endif //CHESS_KING_H
