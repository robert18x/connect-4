#include "game/Game.h"

#include <fmt/ranges.h>

#include <iostream>
#include <limits>
#include <print>
#include <ranges>
#include <utility>

using Connect4Board = Board<Cols(7), Rows(6)>;

template <typename _Board>
void display(_Board board) {
    static constexpr auto red = "\U0001F534"; // 🔴 Red Circle (U+1F534)
    static constexpr auto yellow = "\U0001F7E1"; // 🟡 Yellow Circle (U+1F7E1)
    static constexpr auto empty = "  ";
    
    auto getTokensForRow = [&] (int r) {
        std::array<std::string, _Board::cols> tokens;
        for (unsigned int c = 0; c < _Board::cols; ++c) {
            std::optional<Player> token = board.checkAt(Cols(c), Rows(r));
            if (not token.has_value()) {
                tokens[c] = empty;
            } else if (*token == Player::player1) {
                tokens[c] = red;
            } else {
                tokens[c] = yellow;
            }
        }
        return tokens;
    };

    auto horizontalLine = fmt::format("╠{}════╣", fmt::join(std::views::repeat("", _Board::cols), "════╬"));
    auto footage = fmt::format("╚{}════╝", fmt::join(std::views::repeat("", _Board::cols), "════╩"));

    // header
    std::cout << "   " << fmt::format("{}", fmt::join(std::ranges::iota_view{1u, _Board::cols + 1}, "    ")) << '\n';
    std::cout << "╔" << fmt::format("{}", fmt::join(std::views::repeat("", _Board::cols), "════╦")) << "════╗\n";

    for (int r = _Board::rows - 1; r >= 0; --r) {
        std::array<std::string, _Board::cols> tokens = getTokensForRow(r);

        std::cout << "║ "  << fmt::format("{}", (fmt::join(tokens, " ║ "))) << " ║\n";
        bool isLastRow = (r == 0);
        if (isLastRow) {
            std::cout << footage << '\n';
        } else {
            std::cout << horizontalLine << '\n';
        }
    }
}

bool toggle(bool& b) {
    return !(b = !b);
}

Move<Connect4Board> readMove() {
    static bool player1 = true;
    while (true) {
        std::cout << "column: " << std::flush;
        int col = 0;
        std::cin >> col;
        if (not std::cin.good()) {
            std::cout << "Occured error while reading from stream, try again" << std::endl;
            std::cin.clear();
            std::string ignore;
            std::getline(std::cin, ignore);
        } else if (col < 1 or std::cmp_greater(col, Connect4Board::cols)) {
            std::println("Invalid move! Columns range: [1, {}]", Connect4Board::cols);
        } else {
            return Move<Connect4Board>{
                .position = Connect4Board::Position{
                    .col = static_cast<uint8_t>(col-1)
                },
                .player = toggle(player1) ? Player::player1 : Player::player2
            };
        }
    }
}

void clearConsole() {
    std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
}

void game() {
    Game<Connect4Board> game;
    display(game.getCurrentState());

    GameResult gameResult = GameResult::none;

    while (gameResult == GameResult::none) {
        Move<Connect4Board> nextMove = readMove();

        auto result = game.makeMove(nextMove);

        clearConsole();
        display(game.getCurrentState());

        if (result.has_value()) {
            gameResult = *result;
        } else {
            std::println("\nError: {}\n", result.error());
        }
    }
    std::cout << magic_enum::enum_name(gameResult) << std::endl;
}

int main() {
    try {
        game();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::println("Cached exception: {}", e.what());
        return EXIT_FAILURE;
    }
}
