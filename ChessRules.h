#ifndef CHESS_RULES_H
#define CHESS_RULES_H

#include <string>

namespace ChessRules {

struct CastlingRights {
    bool whiteKingSide = false;
    bool whiteQueenSide = false;
    bool blackKingSide = false;
    bool blackQueenSide = false;
};

bool isKingInCheck(const std::string &board, bool white);
bool isLegalMove(const std::string &board, bool whiteToMove,
                 int fromRow, int fromCol, int toRow, int toCol,
                 CastlingRights rights = {});

} // namespace ChessRules

#endif // CHESS_RULES_H
