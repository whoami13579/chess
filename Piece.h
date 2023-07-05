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
    short getRow();
    short getCol();
    char getType();
    bool selected = false;

private:
    sf::Sprite sprite;
    char type;
    bool taken = false;
    short row, col;

};


#endif //CHESS_PIECE_H
