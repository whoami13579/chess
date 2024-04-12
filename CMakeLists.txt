cmake_minimum_required(VERSION 3.25)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)
# include_directories(${SFML_INCLUDE_DIRS})
include_directories(sfml/2.6.1/include)
# include_directories(usr/local/include)
set(SOURCE_FILES main.cpp
        PieceTexture.cpp
        PieceTexture.h
        Board.cpp Board.h LegalMove.cpp LegalMove.h ChessGame.cpp ChessGame.h Piece.cpp Piece.h Promotion.cpp Promotion.h)

add_executable(chess ${SOURCE_FILES})
target_link_libraries(chess PRIVATE sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
