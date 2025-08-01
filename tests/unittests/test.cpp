#include <snitch/snitch.hpp>
#include "game/Game.h"
#include "game/Bot.h"

using Connect4Board = Board<Cols(6), Rows(4)>;

Move<Connect4Board> move(int col, Player player) {
    return Move<Connect4Board>{
        .position = Connect4Board::Position{
            .col = static_cast<uint8_t>(col)
        },
        .player = player
    };
};

TEST_CASE("Game model test", "[test vertical win]" ) {
    Connect4Board board;

    REQUIRE(board.next(move(0, Player::player1)) == GameResult::none);
    REQUIRE(board.next(move(0, Player::player1)) == GameResult::none);
    REQUIRE(board.next(move(0, Player::player1)) == GameResult::none);
    REQUIRE(board.next(move(0, Player::player1)) == GameResult::player1_wins);
}

TEST_CASE("Game model test", "[test upper left to bottom right win]" ) {
    Connect4Board board;

    // o
    // * o
    // * * o
    // * * * o
    REQUIRE(board.next(move(0, Player::player2)) == GameResult::none);
    REQUIRE(board.next(move(0, Player::player2)) == GameResult::none);
    REQUIRE(board.next(move(0, Player::player2)) == GameResult::none);
    REQUIRE(board.next(move(0, Player::player1)) == GameResult::none);

    REQUIRE(board.next(move(1, Player::player2)) == GameResult::none);
    REQUIRE(board.next(move(1, Player::player2)) == GameResult::none);
    REQUIRE(board.next(move(1, Player::player1)) == GameResult::none);

    REQUIRE(board.next(move(2, Player::player2)) == GameResult::none);
    REQUIRE(board.next(move(2, Player::player1)) == GameResult::none);

    REQUIRE(board.next(move(3, Player::player1)) == GameResult::player1_wins);
}


TEST_CASE("Test bot", "[test]" ) {
    Connect4Board board;

    // Bot<Connect4Board> bot();
    // auto move = bot.negamax(board, Player::player1);

    // REQUIRE(move.player == Player::player1);
    // REQUIRE(move.position.col == 3u);
}