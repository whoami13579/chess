//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_CHESSGAME_H
#define CHESS_CHESSGAME_H

#include <string>
#include "Board.h"
#include "LegalMove.h"
#include "Piece.h"
#include "Promotion.h"

class ChessGame : public sf::Drawable {
public:
    ChessGame(sf::RenderWindow *window, sf::Event *event, sf::Color c1, sf::Color c2);
    void select(int row, int col);

    void restart();
    void createPieces();
    bool turn = true; // white : true, black false

private:
    Board board;
    LegalMove legalMove;
    Promotion promotion;
    Piece pieces[32];

    sf::RectangleShape restartButton; //There isn't button class in sfml, so restartButton in just white area.

    sf::Font font;
    sf::Text restartText; //This is just the text displaied in the white area.

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawPieces(sf::RenderTarget &target, sf::RenderStates states) const;
    void move(int frow, int fcol, int trow, int tcol); //frow : from which row, fcol : from which col, trow : to which row, tcol : to which col
    void generate_moves(int row, int col);

    void wPromotion(Piece *piece, int col);
    void bPromotion(Piece *piece, int col);

    Piece *findPiece(int row, int col);
    bool isCheck(const std::string &bitboard);
    bool wCastleK = true;
    bool wCastleQ = true;
    bool bCastleK = true;
    bool bCastleQ = true;
    bool promote = false;
    std::string fromAtoB(int frow, int fcol, int trow, int tcol);
    sf::RenderWindow *window;
    sf::Event *event;
};


#endif //CHESS_CHESSGAME_H
