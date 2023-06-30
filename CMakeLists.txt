cmake_minimum_required(VERSION 3.25)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
include_directories(/usr/local/include)
set(SOURCE_FILES main.cpp
        pieces/PieceTexture.cpp
        pieces/PieceTexture.h
        pieces/Piece.cpp
        pieces/Piece.h pieces/Bishop.cpp pieces/Bishop.h pieces/King.cpp pieces/King.h pieces/Knight.cpp pieces/Knight.h pieces/Pawn.cpp pieces/Pawn.h pieces/Queen.cpp pieces/Queen.h pieces/Rook.cpp pieces/Rook.h)

add_executable(chess ${SOURCE_FILES})
target_link_libraries(chess sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
