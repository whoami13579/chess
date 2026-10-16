//
// Created by 褚翊喨 on 2023/6/30.
//

#include "PieceTexture.h"

sf::Texture PieceTexture::loadTexture(const char *name) {
    sf::Texture tmp;
    tmp.loadFromFile(std::string(CHESS_ASSET_DIR) + "/" + name);

    return tmp;
}

sf::Texture PieceTexture::whiteKing = PieceTexture::loadTexture("w_king.png");
sf::Texture PieceTexture::blackKing = PieceTexture::loadTexture("b_king.png");
sf::Texture PieceTexture::whiteQueen = PieceTexture::loadTexture("w_queen.png");
sf::Texture PieceTexture::blackQueen = PieceTexture::loadTexture("b_queen.png");
sf::Texture PieceTexture::whiteBishop = PieceTexture::loadTexture("w_bishop.png");
sf::Texture PieceTexture::blackBishop = PieceTexture::loadTexture("b_bishop.png");
sf::Texture PieceTexture::whiteKnight = PieceTexture::loadTexture("w_knight.png");
sf::Texture PieceTexture::blackKnight = PieceTexture::loadTexture("b_knight.png");
sf::Texture PieceTexture::whiteRook = PieceTexture::loadTexture("w_rook.png");
sf::Texture PieceTexture::blackRook = PieceTexture::loadTexture("b_rook.png");
sf::Texture PieceTexture::whitePawn = PieceTexture::loadTexture("w_pawn.png");
sf::Texture PieceTexture::blackPawn = PieceTexture::loadTexture("b_pawn.png");
