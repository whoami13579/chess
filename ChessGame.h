//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_CHESSGAME_H
#define CHESS_CHESSGAME_H

#include "Board.h"
#include "LegalMove.h"
#include "Piece.h"

class ChessGame : public sf::Drawable {
private:
    Board board;
    LegalMove legalMove;
    Piece materials[32];

    sf::RectangleShape restartButton; //There isn't button class in sfml, so restartButton in just white area.

    sf::Font font;
    sf::Text restartText; //This is just the text displaied in the white area.

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawMaterial(sf::RenderTarget &target, sf::RenderStates states) const;

public:
    ChessGame(sf::Color c1, sf::Color c2);
    void test(sf::Event &event, sf::RenderWindow &window, int row, int col);

    void restart();
    void createMaterials();
};


#endif //CHESS_CHESSGAME_H
