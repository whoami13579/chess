//
// Created by 褚翊喨 on 2023/7/1.
//

#include "Piece.h"

Piece::Piece() = default;

Piece::Piece(char type) {
    this->type = type;

    switch (type) {
        case 'B':
            sprite.setTexture(PieceTexture::whiteBishop);
            break;
        case 'b':
            sprite.setTexture(PieceTexture::blackBishop);
            break;
        case 'K':
            sprite.setTexture(PieceTexture::whiteKing);
            break;
        case 'k':
            sprite.setTexture(PieceTexture::blackKing);
            break;
        case 'N':
            sprite.setTexture(PieceTexture::whiteKnight);
            break;
        case 'n':
            sprite.setTexture(PieceTexture::blackKnight);
            break;
        case 'P':
            sprite.setTexture(PieceTexture::whitePawn);
            break;
        case 'p':
            sprite.setTexture(PieceTexture::blackPawn);
            break;
        case 'Q':
            sprite.setTexture(PieceTexture::whiteQueen);
            break;
        case 'q':
            sprite.setTexture(PieceTexture::blackQueen);
            break;
        case 'R':
            sprite.setTexture(PieceTexture::whiteRook);
            break;
        case 'r':
            sprite.setTexture(PieceTexture::blackRook);
            break;
    }

    sprite.setOrigin(sf::Vector2f(float(sprite.getTexture()->getSize().x / 2), float(sprite.getTexture()->getSize().y / 2)));
    sprite.setScale(sf::Vector2f(0.5, 0.5));

}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if(!taken) {
        target.draw(sprite);
    }
}

void Piece::setPosition(int row, int col)
{
    this->row = row;
    this->col = col;

    sprite.setPosition(sf::Vector2f((float)col*80 + 80/2, (float)row*80 + 80/2));
}

short Piece::getRow() {
    return row;
}

short Piece::getCol() {
    return col;
}

char Piece::getType() {
    return type;
}