//
// Created by 褚翊喨 on 2023/6/30.
//

#include "ChessGame.h"

ChessGame::ChessGame(sf::Color c1, sf::Color c2) {
    board = Board(c1, c2);

    font.loadFromFile("../Textures/arial.ttf");

    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(-5.f);
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setPosition(sf::Vector2f(1200.f, 0.f));
    restartButton.setSize(sf::Vector2f(500.f, 50.f));

    restartText.setFont(font);
    restartText.setString("RESTART");
    restartText.setCharacterSize(24);
    restartText.setStyle(sf::Text::Bold);
    restartText.setFillColor(sf::Color::Black);
    restartText.setPosition(restartButton.getPosition().x + 180.f, restartButton.getPosition().y + 10.f);

    createPieces();
    restart();
}

void ChessGame::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(board);
    drawPieces(target, states);
    target.draw(legalMove);
    target.draw(restartButton);
    target.draw(restartText);
}

void ChessGame::restart() {
    board.reset();
    legalMove.reset();
    int tmp;
    bool color = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tmp = (i * 8) + j;
            board.squares[tmp].setPosition(i * 150, j * 150);
            board.squares[tmp].setSize(sf::Vector2f((float)150, (float)150));
            board.squares[tmp].setFillColor((color ? board.c1 : board.c2));

            color = !color;
        }
        color = !color;
    }


    pieces[0].setPosition(7, 4);
    pieces[1].setPosition(0, 4);
    pieces[2].setPosition(7, 3);
    pieces[3].setPosition(0, 3);
    pieces[4].setPosition(7, 2);
    pieces[5].setPosition(7, 5);
    pieces[6].setPosition(0, 2);
    pieces[7].setPosition(0, 5);
    pieces[8].setPosition(7, 1);
    pieces[9].setPosition(7, 6);
    pieces[10].setPosition(0, 1);
    pieces[11].setPosition(0, 6);
    pieces[12].setPosition(7, 0);
    pieces[13].setPosition(7, 7);
    pieces[14].setPosition(0, 0);
    pieces[15].setPosition(0, 7);
    for(int i = 0; i < 8; i++) {
        pieces[i + 16].setPosition(6, i);
    }
    for(int i = 0; i < 8; i++) {
        pieces[i + 24].setPosition(1, i);
    }

    turn = true;
}

void ChessGame::select(sf::Event &event, sf::RenderWindow &window, int row, int col) {
    if((!turn) && ('A' <= board.bitBoard[row*8 + col] && board.bitBoard[row*8 + col] <= 'Z')) {
        return;
    }
    if((turn) && ('a' <= board.bitBoard[row*8 + col] && board.bitBoard[row*8 + col] <= 'z')) {
        return;
    }
    int i;
    for(i = 0; i < 32; i++) {
        if(row == pieces[i].getRow() && col == pieces[i].getCol()) {
            break;
        }
    }
    if((i == 31) && (row != pieces[i].getRow() || col != pieces[0].getCol())) {
        return;
    }

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if ((0 < event.mouseButton.x) && (event.mouseButton.x < 1200) && (0 < event.mouseButton.y) &&
                        (event.mouseButton.y < 1200)) {
//                        legalMove.setMove(row, col);
//                        if(board.bitBoard[(int (event.mouseButton.y/150)*8) + int (event.mouseButton.x/150)])
                        pieces[i].setPosition((int)(event.mouseButton.y/150), (int)(event.mouseButton.x/150));
                        turn = !turn;
                        return;
                    } else if ((1200 < event.mouseButton.x) && (event.mouseButton.x < 1700) &&
                               (0 < event.mouseButton.y) && (event.mouseButton.y < 50)) {
                        restart();
                        return;
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    return;
                }
                else {
                    return;
                }
            }
        }
    }

}

void ChessGame::drawPieces(sf::RenderTarget &target, sf::RenderStates states) const {
    for(auto &piece : pieces) {
        piece.draw(target, states);
    }

//    for(int i = 0; i < 32; i++) {
//        pieces[i].draw(target, states);
//    }
}

void ChessGame::createPieces() {
    pieces[0] = Piece('K');
    pieces[1] = Piece('k');
    pieces[2] = Piece('Q');
    pieces[3] = Piece('q');
    pieces[4] = Piece('B');
    pieces[5] = Piece('B');
    pieces[6] = Piece('b');
    pieces[7] = Piece('b');
    pieces[8] = Piece('N');
    pieces[9] = Piece('N');
    pieces[10] = Piece('n');
    pieces[11] = Piece('n');
    pieces[12] = Piece('R');
    pieces[13] = Piece('R');
    pieces[14] = Piece('r');
    pieces[15] = Piece('r');

    for(int i = 16; i < 24; i++) {
        pieces[i] = Piece('P');
    }
    for(int i = 24; i < 32; i++) {
        pieces[i] = Piece('p');
    }
}