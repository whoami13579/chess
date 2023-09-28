//
// Created by 褚翊喨 on 2023/9/28.
//

#ifndef CHESS_PROMOTION_H
#define CHESS_PROMOTION_H

#include <SFML/Graphics.hpp>
#include "PieceTexture.h"

class Promotion : public sf::Drawable {
public:
    Promotion() = default;
    Promotion(sf::Color color);
    void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
private:
    sf::RectangleShape squares[4];
    sf::Sprite fourPieces[4];

    friend class ChessGame;
};


#endif //CHESS_PROMOTION_H
