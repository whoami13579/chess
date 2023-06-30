//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_PIECETEXTURE_H
#define CHESS_PIECETEXTURE_H

#include <SFML/Graphics.hpp>

class PieceTexture {
public:
    static sf::Texture whiteKing;
    static sf::Texture blackKing;
    static sf::Texture whiteQueen;
    static sf::Texture blackQueen;
    static sf::Texture whiteBishop;
    static sf::Texture blackBishop;
    static sf::Texture whiteKnight;
    static sf::Texture blackKnight;
    static sf::Texture whiteRook;
    static sf::Texture blackRook;
    static sf::Texture whitePawn;
    static sf::Texture blackPawn;

    static sf::Texture loadTexture(const char *name);
};


#endif //CHESS_PIECETEXTURE_H
