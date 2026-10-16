cmake_minimum_required(VERSION 3.16)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.6 COMPONENTS graphics REQUIRED)

set(CHESS_ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Textures" CACHE PATH
        "Directory containing the chess textures and font")
set(CHESS_REQUIRED_ASSETS
        arial.ttf
        b_bishop.png b_king.png b_knight.png b_pawn.png b_queen.png b_rook.png
        w_bishop.png w_king.png w_knight.png w_pawn.png w_queen.png w_rook.png)
foreach(asset IN LISTS CHESS_REQUIRED_ASSETS)
    if(NOT EXISTS "${CHESS_ASSET_DIR}/${asset}" OR IS_DIRECTORY "${CHESS_ASSET_DIR}/${asset}")
        message(FATAL_ERROR "Missing required chess asset: ${CHESS_ASSET_DIR}/${asset}")
    endif()
endforeach()

set(SOURCE_FILES main.cpp
        ChessRules.cpp ChessRules.h
        PieceTexture.cpp
        PieceTexture.h
        Board.cpp Board.h LegalMove.cpp LegalMove.h ChessGame.cpp ChessGame.h Piece.cpp Piece.h Promotion.cpp Promotion.h)

add_executable(chess ${SOURCE_FILES})
target_compile_definitions(chess PRIVATE CHESS_ASSET_DIR="${CHESS_ASSET_DIR}")
target_link_libraries(chess PRIVATE sfml-graphics)

include(CTest)
if(BUILD_TESTING)
    add_executable(chess_rules_tests tests/ChessRulesTests.cpp ChessRules.cpp)
    target_include_directories(chess_rules_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME chess_rules COMMAND chess_rules_tests)
endif()
