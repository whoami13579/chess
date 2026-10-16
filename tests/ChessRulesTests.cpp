#include "ChessRules.h"

#include <iostream>
#include <string>

namespace {

std::string emptyBoard() {
    return std::string(64, ' ');
}

void put(std::string &board, int row, int col, char piece) {
    board[row * 8 + col] = piece;
}

int failures = 0;

void check(bool condition, const char *description) {
    if (!condition) {
        std::cerr << "FAIL: " << description << '\n';
        ++failures;
    }
}

} // namespace

int main() {
    using ChessRules::isKingInCheck;
    using ChessRules::isLegalMove;

    auto board = emptyBoard();
    put(board, 7, 4, 'K'); // e1
    put(board, 6, 4, 'R'); // e2
    put(board, 0, 4, 'r'); // e8
    put(board, 0, 0, 'k');
    check(!isKingInCheck(board, true), "a pinned rook shields its king");
    check(!isLegalMove(board, true, 6, 4, 6, 3), "pinned rook cannot leave the file");
    check(isLegalMove(board, true, 6, 4, 5, 4), "pinned rook can stay on the file");
    put(board, 6, 4, 'N');
    check(!isLegalMove(board, true, 6, 4, 4, 5), "pinned knight cannot expose the king");

    board = emptyBoard();
    put(board, 7, 0, 'K');
    put(board, 0, 4, 'k');
    put(board, 1, 4, 'b');
    put(board, 7, 4, 'R');
    check(!isLegalMove(board, false, 1, 4, 2, 5), "black pinned bishop cannot expose its king");

    board = emptyBoard();
    put(board, 7, 4, 'K');
    put(board, 6, 0, 'P');
    put(board, 0, 4, 'r');
    put(board, 0, 0, 'k');
    check(isKingInCheck(board, true), "rook gives check along an open file");
    check(!isLegalMove(board, true, 6, 0, 5, 0), "unrelated pawn move cannot ignore check");
    check(!isLegalMove(board, true, 7, 4, 6, 4), "king cannot move along an attacked file");
    check(isLegalMove(board, true, 7, 4, 7, 3), "king can escape to a safe square");

    board = emptyBoard();
    put(board, 6, 4, 'K');
    put(board, 4, 3, 'p');
    put(board, 0, 0, 'k');
    check(!isLegalMove(board, true, 6, 4, 5, 4), "king cannot enter a pawn attack");
    put(board, 4, 3, ' ');
    put(board, 4, 4, 'k');
    check(!isLegalMove(board, true, 6, 4, 5, 4), "kings cannot become adjacent");

    board = emptyBoard();
    put(board, 5, 0, 'K');
    put(board, 4, 7, 'p');
    put(board, 0, 4, 'k');
    check(!isKingInCheck(board, true), "pawn attacks do not wrap across the board edge");
    check(!isLegalMove(board, true, 5, 0, 5, -1), "off-board king move is rejected");

    board = emptyBoard();
    put(board, 4, 4, 'K');
    put(board, 0, 0, 'k');
    put(board, 2, 5, 'n');
    check(isKingInCheck(board, true), "knight attack is detected");
    put(board, 2, 5, ' ');
    put(board, 1, 7, 'b');
    check(isKingInCheck(board, true), "bishop attack is detected");
    put(board, 1, 7, ' ');
    put(board, 0, 4, 'q');
    put(board, 0, 0, 'k');
    check(isKingInCheck(board, true), "queen attack is detected");
    board = emptyBoard();
    put(board, 7, 0, 'K');
    put(board, 2, 4, 'k');
    put(board, 3, 3, 'P');
    check(isKingInCheck(board, false), "white pawn attack on black king is detected");

    board = emptyBoard();
    put(board, 7, 0, 'K');
    put(board, 6, 4, 'R');
    put(board, 4, 4, 'k');
    check(!isLegalMove(board, true, 6, 4, 4, 4), "rook cannot capture the enemy king");
    put(board, 6, 4, 'N');
    put(board, 4, 4, ' ');
    put(board, 4, 5, 'k');
    check(!isLegalMove(board, true, 6, 4, 4, 5), "knight cannot capture the enemy king");
    check(!isLegalMove(board, false, 4, 5, 7, 0), "king cannot jump to capture a king");
    put(board, 4, 5, ' ');
    check(!isLegalMove(board, true, 6, 4, 4, 5), "moves require both kings");

    board = emptyBoard();
    put(board, 7, 4, 'K');
    put(board, 7, 7, 'R');
    put(board, 0, 0, 'k');
    ChessRules::CastlingRights rights;
    rights.whiteKingSide = true;
    check(isLegalMove(board, true, 7, 4, 7, 6, rights), "clear kingside castling is legal");
    check(!isLegalMove(board, true, 7, 4, 7, 6), "castling needs unmoved king and rook");
    put(board, 0, 5, 'r');
    check(!isLegalMove(board, true, 7, 4, 7, 6, rights), "castling through check is illegal");
    put(board, 0, 5, ' ');
    put(board, 0, 6, 'r');
    check(!isLegalMove(board, true, 7, 4, 7, 6, rights), "castling into check is illegal");
    put(board, 0, 6, ' ');
    put(board, 0, 4, 'r');
    check(!isLegalMove(board, true, 7, 4, 7, 6, rights), "castling out of check is illegal");

    board = emptyBoard();
    put(board, 7, 4, 'K');
    put(board, 7, 0, 'R');
    put(board, 0, 4, 'k');
    rights = {};
    rights.whiteQueenSide = true;
    check(isLegalMove(board, true, 7, 4, 7, 2, rights), "clear queenside castling is legal");
    put(board, 7, 1, 'N');
    check(!isLegalMove(board, true, 7, 4, 7, 2, rights), "queenside castling needs a clear path");

    board = emptyBoard();
    put(board, 7, 0, 'K');
    put(board, 0, 4, 'k');
    put(board, 0, 7, 'r');
    rights = {};
    rights.blackKingSide = true;
    check(isLegalMove(board, false, 0, 4, 0, 6, rights), "black kingside castling is legal");
    put(board, 7, 5, 'R');
    check(!isLegalMove(board, false, 0, 4, 0, 6, rights), "black cannot castle through check");

    board = emptyBoard();
    put(board, 7, 4, 'K');
    put(board, 0, 4, 'k');
    put(board, 6, 0, 'P');
    check(isLegalMove(board, true, 6, 0, 4, 0), "pawn can move two squares from start");
    put(board, 5, 0, 'N');
    check(!isLegalMove(board, true, 6, 0, 4, 0), "pawn cannot jump over a piece");
    check(!isLegalMove(board, true, 6, 0, 5, 1), "pawn cannot capture an empty square");
    put(board, 6, 0, ' ');
    put(board, 1, 0, 'P');
    check(isLegalMove(board, true, 1, 0, 0, 0), "pawn can reach promotion rank");

    board = std::string("rnbqkbnr") + "pppppppp" + std::string(32, ' ') +
            "PPPPPPPP" + "RNBQKBNR";
    int whiteMoves = 0;
    int blackMoves = 0;
    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromCol = 0; fromCol < 8; ++fromCol) {
            for (int toRow = 0; toRow < 8; ++toRow) {
                for (int toCol = 0; toCol < 8; ++toCol) {
                    whiteMoves += isLegalMove(board, true, fromRow, fromCol, toRow, toCol);
                    blackMoves += isLegalMove(board, false, fromRow, fromCol, toRow, toCol);
                }
            }
        }
    }
    check(whiteMoves == 20, "white has 20 legal opening moves");
    check(blackMoves == 20, "black has 20 legal opening moves");

    if (failures == 0) {
        std::cout << "Chess rules tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
