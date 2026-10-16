#include "ChessRules.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <initializer_list>

namespace ChessRules {
namespace {

bool onBoard(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

int index(int row, int col) {
    return row * 8 + col;
}

bool isWhite(char piece) {
    return piece >= 'A' && piece <= 'Z';
}

bool isBlack(char piece) {
    return piece >= 'a' && piece <= 'z';
}

int direction(int difference) {
    return (difference > 0) - (difference < 0);
}

bool clearPath(const std::string &board, int fromRow, int fromCol,
               int toRow, int toCol) {
    int rowStep = direction(toRow - fromRow);
    int colStep = direction(toCol - fromCol);
    for (int row = fromRow + rowStep, col = fromCol + colStep;
         row != toRow || col != toCol;
         row += rowStep, col += colStep) {
        if (board[index(row, col)] != ' ') {
            return false;
        }
    }
    return true;
}

bool isSquareAttacked(const std::string &board, int row, int col, bool byWhite) {
    int pawnRow = row + (byWhite ? 1 : -1);
    char pawn = byWhite ? 'P' : 'p';
    for (int colStep : {-1, 1}) {
        int pawnCol = col + colStep;
        if (onBoard(pawnRow, pawnCol) && board[index(pawnRow, pawnCol)] == pawn) {
            return true;
        }
    }

    const int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    for (const auto &move : knightMoves) {
        int nextRow = row + move[0];
        int nextCol = col + move[1];
        if (onBoard(nextRow, nextCol) &&
            board[index(nextRow, nextCol)] == (byWhite ? 'N' : 'n')) {
            return true;
        }
    }

    for (int rowStep = -1; rowStep <= 1; ++rowStep) {
        for (int colStep = -1; colStep <= 1; ++colStep) {
            if (rowStep == 0 && colStep == 0) {
                continue;
            }
            int nextRow = row + rowStep;
            int nextCol = col + colStep;
            int distance = 1;
            while (onBoard(nextRow, nextCol)) {
                char piece = board[index(nextRow, nextCol)];
                if (piece != ' ') {
                    if ((byWhite ? isWhite(piece) : isBlack(piece))) {
                        char type = static_cast<char>(std::toupper(static_cast<unsigned char>(piece)));
                        if (type == 'Q' ||
                            (distance == 1 && type == 'K') ||
                            (rowStep == 0 || colStep == 0 ? type == 'R' : type == 'B')) {
                            return true;
                        }
                    }
                    break;
                }
                nextRow += rowStep;
                nextCol += colStep;
                ++distance;
            }
        }
    }
    return false;
}

} // namespace

bool isKingInCheck(const std::string &board, bool white) {
    if (board.size() != 64) {
        return true;
    }
    auto king = board.find(white ? 'K' : 'k');
    if (king == std::string::npos) {
        return true;
    }
    return isSquareAttacked(board, static_cast<int>(king / 8),
                            static_cast<int>(king % 8), !white);
}

bool isLegalMove(const std::string &board, bool whiteToMove,
                 int fromRow, int fromCol, int toRow, int toCol,
                 CastlingRights rights) {
    if (board.size() != 64 || !onBoard(fromRow, fromCol) ||
        !onBoard(toRow, toCol) ||
        (fromRow == toRow && fromCol == toCol) ||
        std::count(board.begin(), board.end(), 'K') != 1 ||
        std::count(board.begin(), board.end(), 'k') != 1) {
        return false;
    }

    char piece = board[index(fromRow, fromCol)];
    char target = board[index(toRow, toCol)];
    if (!(whiteToMove ? isWhite(piece) : isBlack(piece)) ||
        (target != ' ' && (whiteToMove ? isWhite(target) : isBlack(target))) ||
        target == 'K' || target == 'k') {
        return false;
    }

    int rowChange = toRow - fromRow;
    int colChange = toCol - fromCol;
    int absRow = std::abs(rowChange);
    int absCol = std::abs(colChange);
    char type = static_cast<char>(std::toupper(static_cast<unsigned char>(piece)));
    switch (type) {
        case 'P': {
            int step = whiteToMove ? -1 : 1;
            int startRow = whiteToMove ? 6 : 1;
            if (colChange == 0 && target == ' ') {
                if (rowChange != step &&
                    !(fromRow == startRow && rowChange == 2 * step &&
                      board[index(fromRow + step, fromCol)] == ' ')) {
                    return false;
                }
            } else if (absCol != 1 || rowChange != step || target == ' ') {
                return false;
            }
            break;
        }
        case 'N':
            if (!((absRow == 2 && absCol == 1) ||
                  (absRow == 1 && absCol == 2))) {
                return false;
            }
            break;
        case 'B':
            if (absRow == 0 || absRow != absCol ||
                !clearPath(board, fromRow, fromCol, toRow, toCol)) {
                return false;
            }
            break;
        case 'R':
            if (!((rowChange == 0) != (colChange == 0)) ||
                !clearPath(board, fromRow, fromCol, toRow, toCol)) {
                return false;
            }
            break;
        case 'Q':
            if (!((absRow == absCol && absRow != 0) ||
                  ((rowChange == 0) != (colChange == 0))) ||
                !clearPath(board, fromRow, fromCol, toRow, toCol)) {
                return false;
            }
            break;
        case 'K':
            if (absRow <= 1 && absCol <= 1) {
                break;
            }
            if (rowChange != 0 || absCol != 2 || fromCol != 4 ||
                fromRow != (whiteToMove ? 7 : 0)) {
                return false;
            }
            if (colChange > 0 ?
                !(whiteToMove ? rights.whiteKingSide : rights.blackKingSide) :
                !(whiteToMove ? rights.whiteQueenSide : rights.blackQueenSide)) {
                return false;
            }
            {
                int rookCol = colChange > 0 ? 7 : 0;
                int rookTargetCol = colChange > 0 ? 5 : 3;
                if (board[index(fromRow, rookCol)] != (whiteToMove ? 'R' : 'r') ||
                    !clearPath(board, fromRow, fromCol, fromRow, rookCol) ||
                    isKingInCheck(board, whiteToMove)) {
                    return false;
                }
                std::string transit = board;
                transit[index(fromRow, fromCol)] = ' ';
                transit[index(fromRow, fromCol + direction(colChange))] = piece;
                if (isKingInCheck(transit, whiteToMove)) {
                    return false;
                }
                std::string finalBoard = board;
                finalBoard[index(fromRow, fromCol)] = ' ';
                finalBoard[index(fromRow, rookCol)] = ' ';
                finalBoard[index(toRow, toCol)] = piece;
                finalBoard[index(fromRow, rookTargetCol)] = whiteToMove ? 'R' : 'r';
                return !isKingInCheck(finalBoard, whiteToMove);
            }
        default:
            return false;
    }

    std::string finalBoard = board;
    finalBoard[index(fromRow, fromCol)] = ' ';
    finalBoard[index(toRow, toCol)] = piece;
    return !isKingInCheck(finalBoard, whiteToMove);
}

} // namespace ChessRules
