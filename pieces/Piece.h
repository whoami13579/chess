//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include "PieceTexture.h"

class Piece : public sf::Drawable {
protected:
    sf::Sprite sprite;

public:
    Piece();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
    virtual void setPosition(int row, int col);
    virtual int getPositionX();
    virtual int getPositionY();

private:
    int positionX;
    int positionY;
};

#endif //CHESS_PIECE_H
