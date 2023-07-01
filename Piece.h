//
// Created by 褚翊喨 on 2023/7/1.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "PieceTexture.h"

class Piece : public sf::Drawable{
public:
    Piece();
    Piece(char type);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void setPosition(int row, int col);

private:
    sf::Sprite sprite;
    char type;
    bool d = true; // whether to draw
    short row, col;

};


#endif //CHESS_PIECE_H
