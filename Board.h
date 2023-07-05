//
// Created by 褚翊喨 on 2023/6/30.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <SFML/Graphics.hpp>

class Board : public sf::Drawable {
public:
    Board() = default;
    Board(sf::Color c1, sf::Color c2);
    void setColor(int row, int col);
    void reset();

private:
    sf::RectangleShape squares[64];
    sf::Color c1;
    sf::Color c2;
    char bitBoard[64]; //upper case : white pieces, lower case : black pieces.

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    friend class ChessGame;
};


#endif //CHESS_BOARD_H
