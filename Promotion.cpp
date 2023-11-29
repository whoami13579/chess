//
// Created by 褚翊喨 on 2023/9/28.
//

#include "Promotion.h"

Promotion::Promotion(sf::Color color) {
    for(int i = 0; i < 4; i++) {
        squares[i].setSize(sf::Vector2f(80.f, 80.f));
        squares[i].setFillColor(color);
    }
}

void Promotion::draw(sf::RenderTarget &target, sf::RenderStates state) const {
    for(int i = 0; i < 4; i++) {
        target.draw(squares[i]);
    }
    for(int i = 0; i < 4; i++) {
        target.draw(fourPieces[i]);
    }
}
