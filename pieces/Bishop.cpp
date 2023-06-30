//
// Created by 褚翊喨 on 2023/6/30.
//

#include "Bishop.h"

Bishop::Bishop() = default;

Bishop::Bishop(bool color) {
    sprite = sf::Sprite();
    sf::Texture texture;

    if (color) {
        sprite.setTexture(PieceTexture::whiteBishop);
    } else {
        sprite.setTexture(PieceTexture::blackBishop);
    }

    sprite.setOrigin(sf::Vector2f(float(sprite.getTexture()->getSize().x / 2), float(sprite.getTexture()->getSize().y / 2)));
    sprite.setScale(sf::Vector2f(1.25f, 1.2f));
}

void Bishop::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite);
}