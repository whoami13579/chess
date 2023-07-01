//
// Created by 褚翊喨 on 2023/7/1.
//

#ifndef CHESS_MATERIAL_H
#define CHESS_MATERIAL_H

#include "PieceTexture.h"

class Material : public sf::Drawable{
public:
    Material();
    Material(char type);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void setPosition(int row, int col);

private:
    sf::Sprite sprite;
    char type;
    bool d = true; // whether to draw
    short row, col;

};


#endif //CHESS_MATERIAL_H
