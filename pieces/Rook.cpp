//
// Created by 褚翊喨 on 2023/6/30.
//

#include "Rook.h"

Rook::Rook() = default;

Rook::Rook(bool color) {
    sprite = sf::Sprite();
    sf::Texture texture;

    if (color) {
        sprite.setTexture(PieceTexture::whiteRook);
    } else {
        sprite.setTexture(PieceTexture::blackRook);
    }

    sprite.setOrigin(sf::Vector2f(float(sprite.getTexture()->getSize().x / 2), float(sprite.getTexture()->getSize().y / 2)));
    sprite.setScale(sf::Vector2f(1.25f, 1.2f));
}

void Rook::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite);
}