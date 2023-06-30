//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"

class Pieces : public sf::Drawable {
    friend class ChessGame;

public:
    Pieces();

    ~Pieces();

private:
    Piece *pieceArr[32];

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


#endif //CHESS_PIECES_H
