//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_CHESSGAME_H
#define CHESS_CHESSGAME_H

#include <iostream>
#include "Board.h"
#include "LegalMove.h"
#include "Piece.h"

class ChessGame : public sf::Drawable {
public:
    ChessGame(sf::Color c1, sf::Color c2);
    void select(sf::Event &event, sf::RenderWindow &window, int row, int col);

    void restart();
    void createPieces();
    bool turn = true; // white : true, black false

private:
    Board board;
    LegalMove legalMove;
    Piece pieces[32];

    sf::RectangleShape restartButton; //There isn't button class in sfml, so restartButton in just white area.

    sf::Font font;
    sf::Text restartText; //This is just the text displaied in the white area.

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawPieces(sf::RenderTarget &target, sf::RenderStates states) const;
    void move(int frow, int fcol, int trow, int tcol); //frow : from which row, fcol : from which col, trow : to which row, tcol : to which col
    Piece *findPiece(int row, int col);
};


#endif //CHESS_CHESSGAME_H
