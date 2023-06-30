//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_LEGALMOVE_H
#define CHESS_LEGALMOVE_H

#include <SFML/Graphics.hpp>

class LegalMove : public sf::Drawable{
public:
    LegalMove();
    ~LegalMove();
    void setMove(int row, int col);
    void reset();

private:
    sf::CircleShape circles[64];
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    friend class ChessGame;
};


#endif //CHESS_LEGALMOVE_H
