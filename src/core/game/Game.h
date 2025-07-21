#pragma once

#include "utilities/int_types.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <print>
#include <stdexcept>
#include <bit>
#include <magic_enum/magic_enum.hpp>

struct Rows {
    constexpr explicit Rows(std::size_t n) : n(n) {};
    std::size_t n;
    constexpr operator std::size_t () const noexcept {
        return n;
    }
};

struct Cols {
    constexpr explicit Cols(std::size_t n) : n(n) {};
    std::size_t n;
    constexpr operator std::size_t () const noexcept {
        return n;
    }
};

enum class Player : std::uint8_t {
    player1 = 0,
    player2 = 1
};


template <typename _Board>
struct Move {
    _Board::Position position;
    Player player;
};


enum class GameResult {
    error = -2,
    none = -1,
    player1_wins,
    player2_wins,
    draw,
};


template <Cols cols, Rows rows>
class Board {
public:
    explicit constexpr Board() : board(0), cover(0) {}
    constexpr Board(const Board&) = default;
    constexpr Board(Board&&) = default;

    struct Position {
    private:
        template <std::size_t max>
        using uint_type = min_unsigned_type<std::bit_width(max)>;
    public:
        uint_type<cols.n> col;
        uint_type<rows.n> row;
    };

    constexpr bool isFull() const noexcept {
        static constexpr underlying_type fullColumnCover = []() consteval {
            underlying_type fullCover = 0;
            for (unsigned int i = 0; i < cols; ++i) {
                fullCover |= rows.n;
                fullCover <<= std::bit_width(rows.n);
            }
            return fullCover;
        }();

        return (cover ^ fullColumnCover) == 0;
    }

    constexpr bool isValid([[maybe_unused]] Move<Board<cols, rows>> move) noexcept {
        return true; // TODO
    }

    enum class Validation {
        validate,
        no_validate
    };

    constexpr GameResult next(Move<Board<cols, rows>> move) noexcept {
        // board modification - adding token position
        underlying_type movePosition = std::to_underlying(move.player) << (move.position.row * cols + move.position.col);
        board |= movePosition;

        // cover modification - adjustment of a new token position
        cover += 1 << (std::bit_width(rows.n) * move.position.col);

        // check result
        return checkGameResult(move);
    }

    static inline constexpr size_t size = cols * rows;
    using underlying_type = min_unsigned_type<size>;
private:

    constexpr GameResult checkGameResult(const Move<Board<cols, rows>> lastMove) const noexcept {
        // Board:
        //   * - to check if exists and if belongs to player
        //   q - current move
        //   o - not important
        //   e - empty
        // * o o e o o *
        // o * o e o * o
        // o o * e * o o
        // * * * q * * *
        // o o * * * o o
        // o * o * o * o
        // * o o * o o *

        // To check game result there's need to check:
        //  1) vertical positions
        //  2) upper left to bottom right positions
        //  3) horizontal
        //  4) bottom left to upper right positions

        auto& pos = lastMove.position;

        const auto minRow = pos.row >= 3u ? pos.row - 3 : 0u;
        [[maybe_unused]] const auto maxRow = std::min(pos.row + 3Lu, rows.n);
        [[maybe_unused]] const auto minCol = pos.col >= 3u ? pos.col - 3 : 0u;
        [[maybe_unused]] const auto maxCol = std::min(pos.col + 3Lu, rows.n);

        {
            // 1) vertical positions
            if (minRow - pos.row > 4) {
                underlying_type b = board;
                b >>= minRow * cols.n + pos.col;
                static constexpr underlying_type tokenMask = 1u;
                const auto player = std::to_underlying(lastMove.player);
                if ((b & tokenMask) == player and
                    ((b >> cols.n) & tokenMask) == player and
                    ((b >> (cols.n * 2)) & tokenMask) == player and
                    ((b >> (cols.n * 3)) & tokenMask) == player) {
                    return lastMove.player == Player::player1 ? GameResult::player1_wins : GameResult::player2_wins;
                }
            }
        }

        {
            // 2) upper left to bottom right positions
        }

        {
            // 3) horizontal
        }

        {
            // 4) bottom left to upper right positions
        }

        if (isFull()) {
            return GameResult::draw;
        } else {
            return GameResult::none;
        }
    }

    underlying_type board : size;
    underlying_type cover : std::bit_width(rows.n) * cols;
};




template <typename _Board>
class Game {
public:
    explicit constexpr Game() = default;

    using _Move = Move<_Board>;

    constexpr GameResult makeMove(const _Move move) {
        if (lastMove.has_value() and lastMove->player == move.player)
            throw std::runtime_error(std::format("This is not {} move!", magic_enum::enum_name(move.player)));

        if (finished or currentState.isFull())
            throw std::runtime_error("Game is finished!");

        if (not currentState.isValid(move))
            throw std::runtime_error("Invalid move!");

        GameResult result = currentState.next(move);
        lastMove = move;
        if (result == GameResult::player1_wins or result == GameResult::player2_wins or result == GameResult::draw)
            finished = true;
        return result;
    }

private:

    _Board currentState;
    std::optional<_Move> lastMove;
    bool finished = false;
};

