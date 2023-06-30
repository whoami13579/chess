//
// Created by 褚翊喨 on 2023/6/30.
//

#include "King.h"

King::King() = default;

King::King(bool color) {
    sprite = sf::Sprite();
    sf::Texture texture;

    if (color) {
        sprite.setTexture(PieceTexture::whiteKing);
    } else {
        sprite.setTexture(PieceTexture::blackKing);
    }

    sprite.setOrigin(sf::Vector2f(float(sprite.getTexture()->getSize().x / 2), float(sprite.getTexture()->getSize().y / 2)));
    sprite.setScale(sf::Vector2f(1.25f, 1.2f));
}

void King::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite);
}