//
// Created by 褚翊喨 on 2023/6/30.
//

#include "ChessGame.h"

ChessGame::ChessGame(sf::RenderWindow *window, sf::Event *event, sf::Color c1, sf::Color c2) {
    this->window = window;
    this->event = event;
    board = Board(c1, c2);
    promotion = Promotion(sf::Color::Green);

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

    if(promote) {
        target.draw(promotion);
    }
}

void ChessGame::restart() {
    createPieces();
    board.reset();
    legalMove.reset();

    // draw the light squares and the dark squares
    int tmp;
    bool color = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tmp = (i * 8) + j;
            board.squares[tmp].setPosition(float(i * 150), float(j * 150));
            board.squares[tmp].setSize(sf::Vector2f((float) 150, (float) 150));
            board.squares[tmp].setFillColor((color ? board.c1 : board.c2));

            color = !color;
        }
        color = !color;
    }

    // set each piece's position
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
    for (int i = 0; i < 8; i++) {
        pieces[i + 16].setPosition(6, i);
    }
    for (int i = 0; i < 8; i++) {
        pieces[i + 24].setPosition(1, i);
    }

    for (int i = 0; i < 32; i++) {
        pieces[i].taken = false;
        pieces[i].moved = false;
    }

    // the first turn is white's turn
    turn = true;
    promote = false;
    wCastleK = false;
    wCastleK = false;
    bCastleK = false;
    bCastleQ = false;
}

void ChessGame::select(int row, int col) {
    // the user selects white piece on black's turn, return
    if ((!turn) && ('A' <= board.bitBoard[row * 8 + col] && board.bitBoard[row * 8 + col] <= 'Z')) {
        return;
    }

    // the user selects black piece on white's turn, return
    if ((turn) && ('a' <= board.bitBoard[row * 8 + col] && board.bitBoard[row * 8 + col] <= 'z')) {
        return;
    }

    // if user selects no piece, return
    if (board.bitBoard[row * 8 + col] == ' ') {
        return;
    }

    // generate the legal movies and draw the game
    generate_moves(row, col);
    window->draw(*this);
    window->display();

    // int i;
    // for (i = 0; i < 32; i++) {
    //     if (row == pieces[i].getRow() && col == pieces[i].getCol()) {
    //         break;
    //     }
    // }
    // if ((i == 31) && (row != pieces[i].getRow() || col != pieces[0].getCol())) {
    //     return;
    // }

    int x, y; // the position of mouse click

    while (window->isOpen()) {
        while (window->pollEvent(*event)) {
            if (event->type == sf::Event::Closed) {
                window->close();
            }

            if (event->type == sf::Event::MouseButtonPressed) {
                if (event->mouseButton.button == sf::Mouse::Left) {
                    x = event->mouseButton.x;
                    y = event->mouseButton.y;
                    if ((0 < x) && (x < 1200) && (0 < y) && (y < 1200)) {
                        // if the user clicks the same piece, return
                        if(x/150 == col && y/150 == row) {
                            legalMove.reset();
                            return;
                        }

                        // if the square the user clicks is not a legal move, return
                        if(legalMove.bitBoard[(y/150)*8 + (x/150)] == false) {
                            legalMove.reset();
                            return;
                        }


                        move(row, col, (int) (y / 150), (int) (x / 150));
                        return;
                    } else if ((1200 < x) && (x < 1700) && (0 < y) && (y < 50)) {
                        restart();
                        return;
                    }
                } else if (event->mouseButton.button == sf::Mouse::Right) {
                    return;
                } else {
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

    // for (int i = 0; i < 32; i++) {
    //     pieces[i].draw(target, states);
    // }
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

    for (int i = 16; i < 24; i++) {
        pieces[i] = Piece('P');
    }
    for (int i = 24; i < 32; i++) {
        pieces[i] = Piece('p');
    }
}

void ChessGame::move(int frow, int fcol, int trow, int tcol) {
    if (turn && 'A' <= board.bitBoard[trow * 8 + tcol] && board.bitBoard[trow * 8 + tcol] <= 'Z') {
        legalMove.reset();
        return;
    }
    if (!turn && 'a' <= board.bitBoard[trow * 8 + tcol] && board.bitBoard[trow * 8 + tcol] <= 'z') {
        legalMove.reset();
        return;
    }

    if(board.bitBoard[frow * 8 + fcol] == 'K' && trow == 7 && tcol == 6 && legalMove.bitBoard[7 * 8 + 6]) {
        Piece *k = findPiece(frow, fcol);
        Piece *r = findPiece(7, 7);

        if(k->moved || r->moved) {
            goto noCastle;
        }

        k->setPosition(7, 6);
        board.bitBoard[7 * 8 + 6] = 'K';

        r->setPosition(7, 5);
        board.bitBoard[7 * 8 + 5] = 'R';


        board.bitBoard[frow * 8 + fcol] = ' ';
        board.bitBoard[7 * 8 + 7] = ' ';

        k->moved = true;
        r->moved = true;

        legalMove.reset();
        turn = !turn;
        return;
    }

    if(board.bitBoard[frow * 8 + fcol] == 'k' && trow == 0 && tcol == 6 && legalMove.bitBoard[0 * 8 + 6]) {
        Piece *k = findPiece(frow, fcol);
        Piece *r = findPiece(0, 7);

        if(k->moved || r->moved) {
            goto noCastle;
        }

        k->setPosition(0, 6);
        board.bitBoard[0 * 8 + 6] = 'k';

        r->setPosition(0, 5);
        board.bitBoard[0 * 8 + 5] = 'r';


        board.bitBoard[frow * 8 + fcol] = ' ';
        board.bitBoard[0 * 8 + 7] = ' ';

        k->moved = true;
        r->moved = true;

        legalMove.reset();
        turn = !turn;
        return;
    }

    if(board.bitBoard[frow * 8 + fcol] == 'K' && trow == 7 && tcol == 2 && legalMove.bitBoard[7 * 8 + 2]) {
        Piece *k = findPiece(frow, fcol);
        Piece *r = findPiece(7, 0);

        if(k->moved || r->moved) {
            goto noCastle;
        }

        k->setPosition(7, 2);
        board.bitBoard[7 * 8 + 2] = 'K';

        r->setPosition(7, 3);
        board.bitBoard[7 * 8 + 3] = 'R';


        board.bitBoard[frow * 8 + fcol] = ' ';
        board.bitBoard[7 * 8 + 0] = ' ';

        k->moved = true;
        r->moved = true;

        legalMove.reset();
        turn = !turn;
        return;
    }

    if(board.bitBoard[frow * 8 + fcol] == 'k' && trow == 0 && tcol == 2 && legalMove.bitBoard[0 * 8 + 2]) {
        Piece *k = findPiece(frow, fcol);
        Piece *r = findPiece(0, 0);

        if(k->moved || r->moved) {
            goto noCastle;
        }

        k->setPosition(0, 2);
        board.bitBoard[0 * 8 + 2] = 'k';

        r->setPosition(0, 3);
        board.bitBoard[0 * 8 + 3] = 'r';


        board.bitBoard[frow * 8 + fcol] = ' ';
        board.bitBoard[0 * 8 + 0] = ' ';

        k->moved = true;
        r->moved = true;

        legalMove.reset();
        turn = !turn;
        return;
    }

    noCastle:

    if (Piece *p = findPiece(trow, tcol)) {
        p->taken = true;
    }

    if (Piece *p = findPiece(frow, fcol)) {
        p->setPosition(trow, tcol);
        p->moved = true;
        legalMove.reset();
    }

    board.bitBoard[trow * 8 + tcol] = board.bitBoard[frow * 8 + fcol];
    board.bitBoard[frow * 8 + fcol] = ' ';

    if(trow == 0 && board.bitBoard[trow * 8 + tcol] == 'P') {
        wPromotion(findPiece(trow, tcol), tcol);
    }
    
    if(trow == 7 && board.bitBoard[trow * 8 + tcol] == 'p') {
        bPromotion(findPiece(trow, tcol), tcol);
    }

    turn = !turn;
}

Piece *ChessGame::findPiece(int row, int col) {
    for (int i = 0; i < 32; i++) {
        if (!pieces[i].taken && pieces[i].getRow() == row && pieces[i].getCol() == col) {
            return pieces + i;
        }
    }

    return nullptr;
}

void ChessGame::generate_moves(int row, int col) {
    int x = 0, y = 0, tmp;
    Piece *k;
    Piece *r;
    switch (board.bitBoard[row * 8 + col]) {
        case 'P':
            if (row == 6 && board.bitBoard[(row-1)*8 + col] == ' ' && board.bitBoard[(row-2)*8 + col] == ' ') {
                if(!isCheck(fromAtoB(row, col, row-1, col))) {
                    legalMove.setMove(row - 1, col);
                    legalMove.bitBoard[(row-1)*8 + col] = true;
                }
                if(!isCheck(fromAtoB(row, col, row - 2, col))) {
                    legalMove.setMove(row - 2, col);
                    legalMove.bitBoard[(row-2)*8 + col] = true;
                }
            }
            if(row <= 6 && board.bitBoard[(row-1)*8 + col] == ' ') {
                if(!isCheck(fromAtoB(row, col, row - 1, col))) {
                    legalMove.setMove(row - 1, col);
                    legalMove.bitBoard[(row-1)*8 + col] = true;
                }
            }
            if(0 < col && col < 7) {
                if('a' <= board.bitBoard[(row-1) * 8 + col - 1] && board.bitBoard[(row-1) * 8 + col - 1] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col - 1))) {
                        legalMove.setMove(row-1, col-1);
                        legalMove.bitBoard[(row-1)*8 + col - 1] = true;
                    }
                }
                if('a' <= board.bitBoard[(row-1) * 8 + col + 1] && board.bitBoard[(row-1) * 8 + col + 1] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col + 1))) {
                        legalMove.setMove(row-1, col+1);
                        legalMove.bitBoard[(row-1)*8 + col + 1] = true;
                    }
                }
            }
            if(col == 0) {
                if('a' <= board.bitBoard[(row-1) * 8 + col + 1] && board.bitBoard[(row-1) * 8 + col + 1] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col + 1))) {
                        legalMove.setMove(row-1, col+1);
                        legalMove.bitBoard[(row-1)*8 + col + 1] = true;
                    }
                }
            }
            if(col == 7) {
                if('a' <= board.bitBoard[(row-1) * 8 + col - 1] && board.bitBoard[(row-1) * 8 + col - 1] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col - 1))) {
                        legalMove.setMove(row-1, col-1);
                        legalMove.bitBoard[(row-1)*8 + col - 1] = true;
                    }
                }
            }
            break;

        case 'p':
            if (row == 1 && board.bitBoard[(row+1)*8 + col] == ' ' && board.bitBoard[(row+2)*8 + col] == ' ') {
                if(!isCheck(fromAtoB(row, col, row + 1, col))) {
                    legalMove.setMove(row + 1, col);
                    legalMove.bitBoard[(row+1) * 8 + col] = true;
                }
                if(!isCheck(fromAtoB(row, col, row + 2, col))) {
                    legalMove.setMove(row + 2, col);
                    legalMove.bitBoard[(row+2) * 8 + col] = true;
                }
            }
            if(row >= 1 && board.bitBoard[(row+1)*8 + col] == ' '){
                if(!isCheck(fromAtoB(row, col, row + 1 , col))) {
                    legalMove.setMove(row + 1, col);
                    legalMove.bitBoard[(row+1) * 8 + col] = true;
                }
            }
            if(0 < col && col < 7) {
                if('A' <= board.bitBoard[(row+1) * 8 + col - 1] && board.bitBoard[(row+1) * 8 + col - 1] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col - 1))) {
                        legalMove.setMove(row+1, col-1);
                        legalMove.bitBoard[(row+1)*8 + col-1] = true;
                    }
                }
                if('A' <= board.bitBoard[(row+1) * 8 + col + 1] && board.bitBoard[(row+1) * 8 + col + 1] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col + 1))) {
                        legalMove.setMove(row+1, col+1);
                        legalMove.bitBoard[(row+1)*8 + col+1] = true;
                    }
                }
            }
            if(col == 0) {
                if('A' <= board.bitBoard[(row+1) * 8 + col + 1] && board.bitBoard[(row+1) * 8 + col + 1] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col + 1))) {
                        legalMove.setMove(row+1, col+1);
                        legalMove.bitBoard[(row+1)*8 + col+1] = true;
                    }
                }
            }
            if(col == 7) {
                if('A' <= board.bitBoard[(row+1) * 8 + col - 1] && board.bitBoard[(row+1) * 8 + col - 1] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col - 1))) {
                        legalMove.setMove(row+1, col-1);
                        legalMove.bitBoard[(row+1)*8 + col-1] = true;
                    }
                }
            }
            break;
        
        case 'R':
            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y++;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            y = -1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y--;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = 1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = -1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }
            break;

        case 'r':
            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y++;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            y = -1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y--;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = 1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = -1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }
            break;

        case 'K':
            tmp = (row-1)*8 + col;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col))) {
                        legalMove.setMove(row-1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col))) {
                        legalMove.setMove(row-1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row-1)*8 + col+1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col + 1))) {
                        legalMove.setMove(row-1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col + 1))) {
                        legalMove.setMove(row-1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = row*8 + col+1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row, col + 1))) {
                        legalMove.setMove(row, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row, col + 1))) {
                        legalMove.setMove(row, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row+1)*8 + col+1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col + 1))) {
                        legalMove.setMove(row+1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col + 1))) {
                        legalMove.setMove(row+1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row+1)*8 + col;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col))) {
                        legalMove.setMove(row+1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col))) {
                        legalMove.setMove(row+1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row+1)*8 + col-1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col - 1))) {
                        legalMove.setMove(row+1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col - 1))) {
                        legalMove.setMove(row+1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = row*8 + col-1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row, col - 1))) {
                        legalMove.setMove(row, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row, col - 1))) {
                        legalMove.setMove(row, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row-1)*8 + col-1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col - 1))) {
                        legalMove.setMove(row-1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col - 1))) {
                        legalMove.setMove(row-1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            k = findPiece(row, col);
            if(!isCheck(fromAtoB(row, col, row, col)) && k && !k->moved) {
                if(board.bitBoard[7 * 8 + 5] == ' ' && board.bitBoard[7 * 8 + 6] == ' ' && board.bitBoard[7 * 8 + 7] == 'R') {
                    r = findPiece(7, 7);
                    if(r && !r->moved) {
                        if(!isCheck(fromAtoB(7, 4, 7, 5)) && !isCheck(fromAtoB(7, 4, 7, 6))) {
                            legalMove.setMove(7, 6);
                            legalMove.bitBoard[7 * 8 + 6] = true;
                        }
                    }
                }

                if(board.bitBoard[7 * 8 + 3] == ' ' && board.bitBoard[7 * 8 + 2] == ' ' && board.bitBoard[7 * 8 + 1] == ' ' && board.bitBoard[7 * 8 + 0] == 'R') {
                    r = findPiece(7, 0);
                    if(r && !r->moved) {
                        if(!isCheck(fromAtoB(7, 4, 7, 3)) && !isCheck(fromAtoB(7, 4, 7, 2)) && !isCheck(fromAtoB(7, 4, 7, 1))) {
                            legalMove.setMove(7, 2);
                            legalMove.bitBoard[7 * 8 + 2] = true;
                        }
                    }
                }
            }
            break;
        case 'k':
            tmp = (row-1)*8 + col;
            if(0 <= row - 1 && 0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col))) {
                        legalMove.setMove(row-1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col))) {
                        legalMove.setMove(row-1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row-1)*8 + col+1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col + 1))) {
                        legalMove.setMove(row-1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col + 1))) {
                        legalMove.setMove(row-1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = row*8 + col+1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row, col + 1))) {
                        legalMove.setMove(row, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row, col + 1))) {
                        legalMove.setMove(row, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            } 

            tmp = (row+1)*8 + col+1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col + 1))) {
                        legalMove.setMove(row+1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col + 1))) {
                        legalMove.setMove(row+1, col+1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row+1)*8 + col;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col))) {
                        legalMove.setMove(row+1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col))) {
                        legalMove.setMove(row+1, col);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row+1)*8 + col-1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col - 1))) {
                        legalMove.setMove(row+1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + 1, col - 1))) {
                        legalMove.setMove(row+1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = row*8 + col-1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row, col - 1))) {
                        legalMove.setMove(row, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row, col - 1))) {
                        legalMove.setMove(row, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            tmp = (row-1)*8 + col-1;
            if(0 <= tmp && tmp <= 63) {
                if(board.bitBoard[tmp] == ' ') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col - 1))) {
                        legalMove.setMove(row-1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
                else if('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row - 1, col - 1))) {
                        legalMove.setMove(row-1, col-1);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            k = findPiece(row, col);
            if(!isCheck(fromAtoB(row, col, row, col)) && k && !k->moved) {
                if(board.bitBoard[0 * 8 + 5] == ' ' && board.bitBoard[0 * 8 + 6] == ' ' && board.bitBoard[0 * 8 + 7] == 'r') {
                    r = findPiece(0, 7);
                    if(r && !r->moved) {
                        if(!isCheck(fromAtoB(0, 4, 0, 5)) && !isCheck(fromAtoB(0, 4, 0, 6))) {
                            legalMove.setMove(0, 6);
                            legalMove.bitBoard[0 * 8 + 6] = true;
                        }
                    }
                }

                if(board.bitBoard[0 * 8 + 3] == ' ' && board.bitBoard[0 * 8 + 2] == ' ' && board.bitBoard[0 * 8 + 1] == ' ' && board.bitBoard[0 * 8 + 0] == 'r') {
                    r = findPiece(0, 0);
                    if(r && !r->moved) {
                        if(!isCheck(fromAtoB(0, 4, 0, 3)) && !isCheck(fromAtoB(0, 4, 0, 2)) && !isCheck(fromAtoB(0, 4, 0, 1))) {
                            legalMove.setMove(0, 2);
                            legalMove.bitBoard[0 * 8 + 2] = true;
                        }
                    }
                }
            }
            break;
        case 'B':
            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = 1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }
            break;
        case 'b':
            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = 1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }
            break;

        case 'Q':
            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y++;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            y = -1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y--;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = 1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = -1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = 1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }
            break;
        case 'q':
            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y++;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            } 

            y = -1;
            while(0 <= row+y && row+y <=7) {
                tmp = (row+y)*8 + col;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col))) {
                            legalMove.setMove(row+y, col);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                y--;
            }
            tmp = (row+y)*8 + col;
            if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row + y, col))) {
                    legalMove.setMove(row+y, col);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = 1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = -1;
            while(0 <= col+x && col+x <=7) {
                tmp = row*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row, col + x))) {
                            legalMove.setMove(row, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
            }
            tmp = row*8 + col+x;
            if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                if(!isCheck(fromAtoB(row, col, row, col + x))) {
                    legalMove.setMove(row, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = 1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = -1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }

            x = 1;
            y = -1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x++;
                y--;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x)))
                    legalMove.setMove(row+y, col+x);
                    legalMove.bitBoard[tmp] = true;
                }
            }

            x = -1;
            y = 1;
            while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ') {
                        if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                            legalMove.setMove(row+y, col+x);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                    else {
                        break;
                    }
                }
                x--;
                y++;
            }

            if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                tmp = (row+y)*8 + col+x;
                if(0 <= tmp && tmp <= 63 && 'A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z') {
                    if(!isCheck(fromAtoB(row, col, row + y, col + x))) {
                        legalMove.setMove(row+y, col+x);
                        legalMove.bitBoard[tmp] = true;
                    }
                }
            }
            break;
        case 'N':
            if(0 <= row-2 && row-2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                tmp = (row-2)*8 + col+1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row - 2, col + 1))) {
                            legalMove.setMove(row-2, col+1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row-2 && row-2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                tmp = (row-2)*8 + col-1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row - 2, col - 1))) {
                            legalMove.setMove(row-2, col-1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+2 && row+2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                tmp = (row+2)*8 + col+1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row + 2, col + 1))) {
                            legalMove.setMove(row+2, col+1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+2 && row+2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                tmp = (row+2)*8 + col-1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row + 2, col - 1))) {
                            legalMove.setMove(row+2, col-1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+1 && row+1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                tmp = (row+1)*8 + col+2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row + 1, col + 2))) {
                            legalMove.setMove(row+1, col+2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row-1 && row-1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                tmp = (row-1)*8 + col+2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row - 1, col + 2))) {
                            legalMove.setMove(row-1, col+2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+1 && row+1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                tmp = (row+1)*8 + col-2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row + 1, col - 2))) {
                            legalMove.setMove(row+1, col-2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row-1 && row-1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                tmp = (row-1)*8 + col-2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('a' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'z')) {
                        if(!isCheck(fromAtoB(row, col, row - 1, col - 2))) {
                            legalMove.setMove(row-1, col-2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }
            break;
        case 'n':
            if(0 <= row-2 && row-2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                tmp = (row-2)*8 + col+1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row - 2, col + 1))) {
                            legalMove.setMove(row-2, col+1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row-2 && row-2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                tmp = (row-2)*8 + col-1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row - 2, col - 1))) {
                            legalMove.setMove(row-2, col-1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+2 && row+2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                tmp = (row+2)*8 + col+1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row + 2, col + 1))) {
                            legalMove.setMove(row+2, col+1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+2 && row+2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                tmp = (row+2)*8 + col-1;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row + 2, col - 1))) {
                            legalMove.setMove(row+2, col-1);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+1 && row+1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                tmp = (row+1)*8 + col+2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row + 1, col + 2))) {
                            legalMove.setMove(row+1, col+2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row-1 && row-1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                tmp = (row-1)*8 + col+2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row - 1, col + 2))) {
                            legalMove.setMove(row-1, col+2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row+1 && row+1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                tmp = (row+1)*8 + col-2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row + 1, col - 2))) {
                            legalMove.setMove(row+1, col-2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }

            if(0 <= row-1 && row-1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                tmp = (row-1)*8 + col-2;
                if(0 <= tmp && tmp <= 63) {
                    if(board.bitBoard[tmp] == ' ' || ('A' <= board.bitBoard[tmp] && board.bitBoard[tmp] <= 'Z')) {
                        if(!isCheck(fromAtoB(row, col, row - 1, col - 2))) {
                            legalMove.setMove(row-1, col-2);
                            legalMove.bitBoard[tmp] = true;
                        }
                    }
                }
            }
            break;
    }
}

bool ChessGame::isCheck(const std::string &bitboard) {
    int x = 0, y = 0, tmp;

    if(turn) {
        for(int row = 0; row < 8; row++) {
            for(int col = 0; col < 8; col++) {
                switch(bitboard[row * 8 + col]) {
                    case 'p':
                        tmp = (row + 1) * 8 + col + 1;
                        if(row + 1 <= 7 && 0 <= tmp && tmp < 64 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        tmp = (row + 1) * 8 + col - 1;
                        if(row + 1 <= 7 && 0 <= tmp && tmp < 64 && bitboard[tmp] == 'K') {
                            return true;
                        }
                        break;
                    
                    case 'r':
                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y++;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        y = -1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y--;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        x = 1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        x = -1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }
                        break;
                    
                    case 'k':
                        tmp = (row-1)*8 + col;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = (row-1)*8 + col+1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = row*8 + col+1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = (row+1)*8 + col+1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = (row+1)*8 + col;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = (row+1)*8 + col-1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = row*8 + col-1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        tmp = (row-1)*8 + col-1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'K') {
                                return true;
                            }
                        }
                        break;

                    case 'b':
                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        x = -1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        x = 1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        x = -1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }
                        break;

                    case 'q':
                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y++;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        y = -1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y--;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        x = 1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        x = -1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                            return true;
                        }

                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        x = -1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        x = 1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }

                        x = -1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'K') {
                                return true;
                            }
                        }
                        break;
                    
                    case 'n':
                        if(0 <= row-2 && row-2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                            tmp = (row-2)*8 + col+1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row-2 && row-2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                            tmp = (row-2)*8 + col-1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+2 && row+2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                            tmp = (row+2)*8 + col+1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+2 && row+2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                            tmp = (row+2)*8 + col-1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+1 && row+1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                            tmp = (row+1)*8 + col+2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row-1 && row-1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                            tmp = (row-1)*8 + col+2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+1 && row+1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                            tmp = (row+1)*8 + col-2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row-1 && row-1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                            tmp = (row-1)*8 + col-2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'K') {
                                    return true;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
    else {
        for(int row = 0; row < 8; row++) {
            for(int col = 0; col < 8; col++) {
                switch(bitboard[row * 8 + col]) {
                    case 'P':
                        tmp = (row - 1) * 8 + col + 1;
                        if(0 <= row - 1 && 0 <= tmp && tmp < 64 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        tmp = (row - 1) * 8 + col - 1;
                        if(row - 1 <= 7 && 0 <= tmp && tmp < 64 && bitboard[tmp] == 'k') {
                            return true;
                        }
                        break;

                    case 'R':
                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y++;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        y = -1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y--;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        x = 1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        x = -1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }
                    break;

                    case 'K':
                        tmp = (row-1)*8 + col;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = (row-1)*8 + col+1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = row*8 + col+1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = (row+1)*8 + col+1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = (row+1)*8 + col;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = (row+1)*8 + col-1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = row*8 + col-1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        tmp = (row-1)*8 + col-1;
                        if(0 <= tmp && tmp <= 63) {
                            if(bitboard[tmp] == ' ') {
                                ;
                            }
                            else if(bitboard[tmp] == 'k') {
                                return true;
                            }
                        }
                        break;

                    case 'B':
                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        x = -1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        x = 1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        x = -1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }
                        break;

                    case 'Q':
                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y++;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        y = -1;
                        while(0 <= row+y && row+y <=7) {
                            tmp = (row+y)*8 + col;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            y--;
                        }
                        tmp = (row+y)*8 + col;
                        if(0 <= row+y && row+y <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        x = 1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        x = -1;
                        while(0 <= col+x && col+x <=7) {
                            tmp = row*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                        }
                        tmp = row*8 + col+x;
                        if(0 <= col+x && col+x <= 7 && 0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                            return true;
                        }

                        x = 1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        x = -1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        x = 1;
                        y = -1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x++;
                            y--;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }

                        x = -1;
                        y = 1;
                        while(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == ' ') {
                                    ;
                                }
                                else {
                                    break;
                                }
                            }
                            x--;
                            y++;
                        }

                        if(0 <= row+y && row+y <=7 && 0 <= col+x && col+x <= 7) {
                            tmp = (row+y)*8 + col+x;
                            if(0 <= tmp && tmp <= 63 && bitboard[tmp] == 'k') {
                                return true;
                            }
                        }
                        break;

                    case 'N':
                        if(0 <= row-2 && row-2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                            tmp = (row-2)*8 + col+1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row-2 && row-2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                            tmp = (row-2)*8 + col-1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+2 && row+2 <= 7 && 0 <= col+1 && col+1 <= 7) {
                            tmp = (row+2)*8 + col+1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+2 && row+2 <= 7 && 0 <= col-1 && col-1 <= 7) {
                            tmp = (row+2)*8 + col-1;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+1 && row+1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                            tmp = (row+1)*8 + col+2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row-1 && row-1 <= 7 && 0 <= col+2 && col+2 <= 7) {
                            tmp = (row-1)*8 + col+2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row+1 && row+1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                            tmp = (row+1)*8 + col-2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }

                        if(0 <= row-1 && row-1 <= 7 && 0 <= col-2 && col-2 <= 7) {
                            tmp = (row-1)*8 + col-2;
                            if(0 <= tmp && tmp <= 63) {
                                if(bitboard[tmp] == 'k') {
                                    return true;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }

    return false;
}

std::string ChessGame::fromAtoB(int frow, int fcol, int trow, int tcol) {
    std::string str = board.bitBoard;
    str[trow * 8 + tcol] = str[frow * 8 + fcol];
    str[frow * 8 + fcol] = ' ';
    return str;
}

void ChessGame::wPromotion(Piece *piece, int col) {
    for(int i = 0; i < 4; i++) {
        promotion.squares[i].setPosition(sf::Vector2f(col * 150.f, (i) * 150.f));
    }

    promotion.fourPieces[0].setTexture(PieceTexture::whiteQueen);
    promotion.fourPieces[1].setTexture(PieceTexture::whiteKnight);
    promotion.fourPieces[2].setTexture(PieceTexture::whiteRook);
    promotion.fourPieces[3].setTexture(PieceTexture::whiteBishop);

    for(int i = 0; i < 4; i++) {
        promotion.fourPieces[i].setOrigin(sf::Vector2f(float(promotion.fourPieces[i].getTexture()->getSize().x / 2), float(promotion.fourPieces[i].getTexture()->getSize().y / 2)));
        promotion.fourPieces[i].setScale(sf::Vector2f(1, 1));
        promotion.fourPieces[i].setPosition(sf::Vector2f((float)col*150 + 150/2, (float)i*150 + 150/2));
    }

    promote = true;

    window->draw(*this);
    window->display();

    int x, y;

    while(window->isOpen()) {
        while(window->pollEvent(*event)) {
            if(event->type == sf::Event::Closed) {
                window->close();
            }

            if(event->type == sf::Event::MouseButtonPressed) {
                if(event->mouseButton.button == sf::Mouse::Left) {
                    x = event->mouseButton.x;
                    y = event->mouseButton.y;

                    if(y / 150 == 0) {
                        board.bitBoard[0 * 8 + col] = 'Q';
                        *piece = Piece('Q');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if(y / 150 == 1) {
                        board.bitBoard[0 * 8 + col] = 'N';
                        *piece = Piece('N');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if(y / 150 == 2) {
                        board.bitBoard[0 * 8 + col] = 'R';
                        *piece = Piece('R');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if(y / 150 == 3) {
                        board.bitBoard[0 * 8 + col] = 'B';
                        *piece = Piece('B');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if((1200 < x) && (x < 1700) && (0 < y) && (y < 50)) {
                        restart();
                        promote = false;
                        return;
                    }
                }
            }
        }
    }

    promote = false;
}

void ChessGame::bPromotion(Piece *piece, int col) {
    for(int i = 0; i < 4; i++) {
        promotion.squares[i].setPosition(sf::Vector2f(col * 150.f, (i + 4) * 150.f));
    }

    promotion.fourPieces[0].setTexture(PieceTexture::blackBishop);
    promotion.fourPieces[1].setTexture(PieceTexture::blackRook);
    promotion.fourPieces[2].setTexture(PieceTexture::blackKnight);
    promotion.fourPieces[3].setTexture(PieceTexture::blackQueen);

    for(int i = 0; i < 4; i++) {
        promotion.fourPieces[i].setOrigin(sf::Vector2f(float(promotion.fourPieces[i].getTexture()->getSize().x / 2), float(promotion.fourPieces[i].getTexture()->getSize().y / 2)));
        promotion.fourPieces[i].setScale(sf::Vector2f(1, 1));
        promotion.fourPieces[i].setPosition(sf::Vector2f((float)col*150 + 150/2, (float)(i + 4)*150 + 150/2));
    }

    promote = true;

    window->draw(*this);
    window->display();

    int x, y;

    while(window->isOpen()) {
        while(window->pollEvent(*event)) {
            if(event->type == sf::Event::Closed) {
                window->close();
            }

            if(event->type == sf::Event::MouseButtonPressed) {
                if(event->mouseButton.button == sf::Mouse::Left) {
                    x = event->mouseButton.x;
                    y = event->mouseButton.y;

                    if(y / 150 == 7) {
                        board.bitBoard[7 * 8 + col] = 'q';
                        *piece = Piece('q');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if(y / 150 == 6) {
                        board.bitBoard[7 * 8 + col] = 'n';
                        *piece = Piece('n');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if(y / 150 == 5) {
                        board.bitBoard[7 * 8 + col] = 'r';
                        *piece = Piece('r');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if(y / 150 == 4) {
                        board.bitBoard[7 * 8 + col] = 'b';
                        *piece = Piece('b');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if((1200 < x) && (x < 1700) && (0 < y) && (y < 50)) {
                        restart();
                        promote = false;
                        return;
                    }
                }
            }
        }
    }

    promote = false;
}
