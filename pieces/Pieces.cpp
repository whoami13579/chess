//
// Created by 褚翊喨 on 2023/6/30.
//

#include "Pieces.h"

Pieces::Pieces() {
    pieceArr[0] = new King(true);
    pieceArr[1] = new King(false);
    pieceArr[2] = new Queen(true);
    pieceArr[3] = new Queen(false);
    pieceArr[4] = new Bishop(true);
    pieceArr[5] = new Bishop(true);
    pieceArr[6] = new Bishop(false);
    pieceArr[7] = new Bishop(false);
    pieceArr[8] = new Knight(true);
    pieceArr[9] = new Knight(true);
    pieceArr[10] = new Knight(false);
    pieceArr[11] = new Knight(false);
    pieceArr[12] = new Rook(true);
    pieceArr[13] = new Rook(true);
    pieceArr[14] = new Rook(false);
    pieceArr[15] = new Rook(false);
    for(int i = 16; i < 24; i++) {
        pieceArr[i] = new Pawn(true);
    }
    for(int i = 24; i < 32; i++) {
        pieceArr[i] = new Pawn(false);
    }
}

Pieces::~Pieces() {
    for(int i = 0; i < 32; i++) {
        delete pieceArr[i];
    }
}

void Pieces::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (int i = 0; i < 32; i++) {
        target.draw(*(pieceArr[i]));
    }
}