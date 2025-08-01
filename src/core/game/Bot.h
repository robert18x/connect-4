#pragma once

#include "Board.h"

#include <algorithm>
#include <ranges>
#include <iostream>
#include <limits>
#include <print>

template <typename _Board>
class Bot {
public:
    explicit constexpr Bot(Player player) : player(player), opponent(other(player)) {}

    using Move = typename _Board::BoardMove;

    constexpr Move negamax(_Board board) {
        int8_t bestScore = std::numeric_limits<int8_t>::min();
        Move bestMove{};

        const auto positions = board.getAvailableMovePositions();

        const std::size_t nPosAfterSymatrion = [&] {
            if (board.getNumberOfOccupiedTokens() == 0) 
                return (positions.size() + 1) / 2;
            else
                return positions.size();
        }();
        auto searchedPositions = positions
            | std::views::take(nPosAfterSymatrion)
            | std::views::reverse;

        for (typename _Board::Position nextMovePosition : searchedPositions) {
            Move nextMove = Move{
                .position = nextMovePosition,
                .player = player
            };
            int8_t alpha = std::numeric_limits<int8_t>::min();
            int8_t beta = std::numeric_limits<int8_t>::max();
            int8_t depth = _Board::size - board.getNumberOfOccupiedTokens();

            int8_t score = - negamaxImpl(board, nextMove, depth, alpha, beta);
            if (score > bestScore) {
                bestScore = score;
                bestMove = nextMove;
            }
        }
        return bestMove;
    }

private:
    constexpr int8_t negamaxImpl(_Board board, Move currentMove, int8_t depth, int8_t alpha, int8_t beta) {
        GameResult gameResult = board.next(currentMove);

        if (gameResult == GameResult::draw) {
            return 0;
        } else if (std::to_underlying(gameResult) == std::to_underlying(player)) {
            return (depth + 1) / 2;
        } else if (std::to_underlying(gameResult) == std::to_underlying(opponent)) {
            return - (depth + 1) / 2;
        }

        int8_t bestScore = std::numeric_limits<int8_t>::min();
        // [[maybe_unused]] Move bestMove{};

        for (typename _Board::Position nextMovePosition : board.getAvailableMovePositions()) {
            Move nextMove = Move{
                .position = nextMovePosition,
                .player = other(currentMove.player)
            };
            int8_t score = - negamaxImpl(board, nextMove, depth - 1, -beta, -alpha);
            if (score > bestScore) {
                bestScore = score;
                // bestMove = nextMove;
            }
            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                break;
            }
        }
        return bestScore;
    }

    static constexpr inline int8_t negamaxColor(Player player) noexcept {
        return player == Player::player1 ? 1 : -1;
    }

    static constexpr inline Player other(Player player) noexcept {
        return player == Player::player1 ? Player::player2 : Player::player1;
    }

    const Player player;
    const Player opponent;
};
