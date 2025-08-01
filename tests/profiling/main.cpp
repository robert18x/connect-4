#include "game/Game.h"
#include "game/Bot.h"


#include <limits>
#include <ranges>
#include <utility>

using Connect4Board = Board<Cols(4), Rows(6)>;


int main() {
    try {
        Connect4Board board;
        Bot<Connect4Board> bot(Player::player1);
        auto move = bot.negamax(board);
        auto result = board.next(move);
        return std::to_underlying(result);
    } catch (const std::exception& e) {
        std::println("Cached exception: {}", e.what());
        return EXIT_FAILURE;
    }
}
