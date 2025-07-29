#pragma once

#include "Board.h"

#include <magic_enum/magic_enum.hpp>

#include <expected>
#include <optional>
#include <print>

template <typename _Board>
class Game {
public:
    explicit constexpr Game() = default;

    using _Move = Move<_Board>;
    using Board = _Board;

    constexpr std::expected<GameResult, std::string> makeMove(const _Move move) {
        if (lastMove.has_value() and lastMove->player == move.player)
            return std::unexpected(std::format("This is not {} move!", magic_enum::enum_name(move.player)));

        if (finished or currentState.isFull())
            return std::unexpected("Game is finished!");

        if (not currentState.validate(move.position))
            return std::unexpected("Invalid move position!");

        GameResult result = currentState.next(move);
        lastMove = move;
        if (result == GameResult::player1_wins or result == GameResult::player2_wins or result == GameResult::draw)
            finished = true;
        return result;
    }

    constexpr Board getCurrentState() const noexcept {
        return currentState;
    }

private:

    _Board currentState;
    std::optional<_Move> lastMove;
    bool finished = false;
};
