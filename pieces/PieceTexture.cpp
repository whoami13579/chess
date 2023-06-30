//
// Created by 褚翊喨 on 2023/6/30.
//

#include "PieceTexture.h"

sf::Texture PieceTexture::loadTexture(const char *name) {
    sf::Texture tmp;
    tmp.loadFromFile(name);

    return tmp;
}

sf::Texture PieceTexture::whiteKing = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/w_king.png");
sf::Texture PieceTexture::blackKing = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/b_king.png");
sf::Texture PieceTexture::whiteQueen = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/w_queen.png");
sf::Texture PieceTexture::blackQueen = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/b_queen.png");
sf::Texture PieceTexture::whiteBishop = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/w_bishop.png");
sf::Texture PieceTexture::blackBishop = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/b_bishop.png");
sf::Texture PieceTexture::whiteKnight = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/w_knight.png");
sf::Texture PieceTexture::blackKnight = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/b_knight.png");
sf::Texture PieceTexture::whiteRook = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/w_rook.png");
sf::Texture PieceTexture::blackRook = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/b_rook.png");
sf::Texture PieceTexture::whitePawn = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/w_pawn.png");
sf::Texture PieceTexture::blackPawn = PieceTexture::loadTexture("/Users/mba/myCode/chess/Textures/b_pawn.png");