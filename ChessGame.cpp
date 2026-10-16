//
// Created by 褚翊喨 on 2023/6/30.
//

#include "ChessGame.h"

ChessGame::ChessGame(sf::RenderWindow *window, sf::Event *event, sf::Color c1, sf::Color c2) {
    this->window = window;
    this->event = event;
    board = Board(c1, c2);
    promotion = Promotion(sf::Color::Green);

    font.loadFromFile(std::string(CHESS_ASSET_DIR) + "/arial.ttf");

    restartButton.setFillColor(sf::Color::White);
    restartButton.setOutlineThickness(-5.f);
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setPosition(sf::Vector2f(640.f, 0.f));
    restartButton.setSize(sf::Vector2f(250.f, 50.f));

    restartText.setFont(font);
    restartText.setString("RESTART");
    restartText.setCharacterSize(24);
    restartText.setStyle(sf::Text::Bold);
    restartText.setFillColor(sf::Color::Black);
    restartText.setPosition(restartButton.getPosition().x + 65.f, restartButton.getPosition().y + 10.f);

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
            board.squares[tmp].setPosition(float(i * 80), float(j * 80));
            board.squares[tmp].setSize(sf::Vector2f((float) 80, (float) 80));
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
}

void ChessGame::select(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return;
    }

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
                    if ((0 < x) && (x < 640) && (0 < y) && (y < 640)) {
                        // if the user clicks the same piece, return
                        if(x/80 == col && y/80 == row) {
                            legalMove.reset();
                            return;
                        }

                        // if the square the user clicks is not a legal move, return
                        if(legalMove.bitBoard[(y/80)*8 + (x/80)] == false) {
                            legalMove.reset();
                            return;
                        }


                        move(row, col, (int) (y / 80), (int) (x / 80));
                        return;
                    } else if ((640 < x) && (x < 890) && (0 < y) && (y < 50)) {
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
    if (frow < 0 || frow >= 8 || fcol < 0 || fcol >= 8 ||
        trow < 0 || trow >= 8 || tcol < 0 || tcol >= 8 ||
        !legalMove.bitBoard[trow * 8 + tcol] ||
        !ChessRules::isLegalMove(board.bitBoard, turn, frow, fcol, trow, tcol,
                                 castlingRights())) {
        legalMove.reset();
        return;
    }

    Piece *moving = findPiece(frow, fcol);
    Piece *captured = findPiece(trow, tcol);
    if (!moving || (board.bitBoard[trow * 8 + tcol] != ' ' && !captured)) {
        legalMove.reset();
        return;
    }

    char type = board.bitBoard[frow * 8 + fcol];
    if ((type == 'K' || type == 'k') && frow == trow &&
        (tcol == fcol + 2 || tcol == fcol - 2)) {
        int rookFromCol = tcol > fcol ? 7 : 0;
        int rookToCol = tcol > fcol ? 5 : 3;
        Piece *rook = findPiece(frow, rookFromCol);
        if (!rook) {
            legalMove.reset();
            return;
        }
        moving->setPosition(trow, tcol);
        rook->setPosition(frow, rookToCol);
        moving->moved = true;
        rook->moved = true;
        board.bitBoard[frow * 8 + fcol] = ' ';
        board.bitBoard[frow * 8 + rookFromCol] = ' ';
        board.bitBoard[trow * 8 + tcol] = type;
        board.bitBoard[frow * 8 + rookToCol] = turn ? 'R' : 'r';
        legalMove.reset();
        turn = !turn;
        return;
    }

    if (captured) {
        captured->taken = true;
    }
    moving->setPosition(trow, tcol);
    moving->moved = true;
    board.bitBoard[trow * 8 + tcol] = type;
    board.bitBoard[frow * 8 + fcol] = ' ';
    legalMove.reset();

    if (trow == 0 && type == 'P') {
        wPromotion(moving, tcol);
    } else if (trow == 7 && type == 'p') {
        bPromotion(moving, tcol);
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

ChessRules::CastlingRights ChessGame::castlingRights() const {
    ChessRules::CastlingRights rights;
    if (!pieces[0].taken && !pieces[0].moved &&
        pieces[0].row == 7 && pieces[0].col == 4 && board.bitBoard[60] == 'K') {
        rights.whiteKingSide = !pieces[13].taken && !pieces[13].moved &&
                               pieces[13].row == 7 && pieces[13].col == 7 &&
                               board.bitBoard[63] == 'R';
        rights.whiteQueenSide = !pieces[12].taken && !pieces[12].moved &&
                                pieces[12].row == 7 && pieces[12].col == 0 &&
                                board.bitBoard[56] == 'R';
    }
    if (!pieces[1].taken && !pieces[1].moved &&
        pieces[1].row == 0 && pieces[1].col == 4 && board.bitBoard[4] == 'k') {
        rights.blackKingSide = !pieces[15].taken && !pieces[15].moved &&
                               pieces[15].row == 0 && pieces[15].col == 7 &&
                               board.bitBoard[7] == 'r';
        rights.blackQueenSide = !pieces[14].taken && !pieces[14].moved &&
                                pieces[14].row == 0 && pieces[14].col == 0 &&
                                board.bitBoard[0] == 'r';
    }
    return rights;
}

void ChessGame::generate_moves(int row, int col) {
    legalMove.reset();
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return;
    }
    const auto rights = castlingRights();
    for (int toRow = 0; toRow < 8; ++toRow) {
        for (int toCol = 0; toCol < 8; ++toCol) {
            if (ChessRules::isLegalMove(board.bitBoard, turn, row, col,
                                        toRow, toCol, rights)) {
                legalMove.setMove(toRow, toCol);
            }
        }
    }
}

void ChessGame::wPromotion(Piece *piece, int col) {
    for(int i = 0; i < 4; i++) {
        promotion.squares[i].setPosition(sf::Vector2f(col * 80.f, (i) * 80.f));
    }

    promotion.fourPieces[0].setTexture(PieceTexture::whiteQueen);
    promotion.fourPieces[1].setTexture(PieceTexture::whiteKnight);
    promotion.fourPieces[2].setTexture(PieceTexture::whiteRook);
    promotion.fourPieces[3].setTexture(PieceTexture::whiteBishop);

    for(int i = 0; i < 4; i++) {
        promotion.fourPieces[i].setOrigin(sf::Vector2f(float(promotion.fourPieces[i].getTexture()->getSize().x / 2), float(promotion.fourPieces[i].getTexture()->getSize().y / 2)));
        promotion.fourPieces[i].setScale(sf::Vector2f(0.5, 0.5));
        promotion.fourPieces[i].setPosition(sf::Vector2f((float)col*80 + 80/2, (float)i*80 + 80/2));
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

                    if(y / 80 == 0) {
                        board.bitBoard[0 * 8 + col] = 'Q';
                        *piece = Piece('Q');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if(y / 80 == 1) {
                        board.bitBoard[0 * 8 + col] = 'N';
                        *piece = Piece('N');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if(y / 80 == 2) {
                        board.bitBoard[0 * 8 + col] = 'R';
                        *piece = Piece('R');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if(y / 80 == 3) {
                        board.bitBoard[0 * 8 + col] = 'B';
                        *piece = Piece('B');
                        piece->setPosition(0, col);
                        promote = false;
                        return;
                    }
                    else if((640 < x) && (x < 890) && (0 < y) && (y < 50)) {
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
        promotion.squares[i].setPosition(sf::Vector2f(col * 80.f, (i + 4) * 80.f));
    }

    promotion.fourPieces[0].setTexture(PieceTexture::blackBishop);
    promotion.fourPieces[1].setTexture(PieceTexture::blackRook);
    promotion.fourPieces[2].setTexture(PieceTexture::blackKnight);
    promotion.fourPieces[3].setTexture(PieceTexture::blackQueen);

    for(int i = 0; i < 4; i++) {
        promotion.fourPieces[i].setOrigin(sf::Vector2f(float(promotion.fourPieces[i].getTexture()->getSize().x / 2), float(promotion.fourPieces[i].getTexture()->getSize().y / 2)));
        promotion.fourPieces[i].setScale(sf::Vector2f(0.5, 0.5));
        promotion.fourPieces[i].setPosition(sf::Vector2f((float)col*80 + 80/2, (float)(i + 4)*80 + 80/2));
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

                    if(y / 80 == 7) {
                        board.bitBoard[7 * 8 + col] = 'q';
                        *piece = Piece('q');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if(y / 80 == 6) {
                        board.bitBoard[7 * 8 + col] = 'n';
                        *piece = Piece('n');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if(y / 80 == 5) {
                        board.bitBoard[7 * 8 + col] = 'r';
                        *piece = Piece('r');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if(y / 80 == 4) {
                        board.bitBoard[7 * 8 + col] = 'b';
                        *piece = Piece('b');
                        piece->setPosition(7, col);
                        promote = false;
                        return;
                    }
                    else if((640 < x) && (x < 890) && (0 < y) && (y < 50)) {
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
