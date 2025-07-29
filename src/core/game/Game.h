#pragma once

#include "utilities/int_types.h"

#include <magic_enum/magic_enum.hpp>

#include <bit>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <print>
#include <stdexcept>

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
    none = -1,
    player1_wins,
    player2_wins,
    draw,
};


template <Cols _cols, Rows _rows>
class Board {
public:
    explicit constexpr Board() : board(0), cover(0) {}
    constexpr Board(const Board&) = default;
    constexpr Board(Board&&) = default;

    static inline constexpr size_t size = _cols * _rows;
    static inline constexpr size_t cols = _cols;
    static inline constexpr size_t rows = _rows;

    using underlying_type = min_unsigned_type<size>;
    using BoardMove = Move<Board<_cols, _rows>>;

    struct Position {
    private:
        template <std::size_t max>
        using uint_type = min_unsigned_type<std::bit_width(max)>;
    public:
        uint_type<cols> col;
    };

    constexpr std::optional<Player> checkAt(Cols col, Rows row) const noexcept {
        if (col > cols or row > rows) {
            return std::nullopt;
        }
        underlying_type columnFill = (cover >> (col * singleColumnCoverBits)) & columnCoverMask;
        if (columnFill <= row) {
            return std::nullopt;
        }
        underlying_type positionMask = 1u;
        underlying_type position = (board >> (row * cols + col)) & positionMask;
        if (position == std::to_underlying(Player::player1)) {
            return Player::player1;
        } else {
            return Player::player2;
        }
    }

    constexpr bool isFull() const noexcept {
        static constexpr underlying_type fullColumnCover = []() consteval {
            underlying_type fullCover = 0;
            for (unsigned int i = 0; i < cols; ++i) {
                fullCover |= rows + 1;
                fullCover <<= singleColumnCoverBits;
            }
            return fullCover;
        }();

        return cover == fullColumnCover;
    }

    constexpr bool validate([[maybe_unused]] BoardMove move) noexcept {
        return true; // TODO
    }

    enum class Validation {
        validate,
        no_validate
    };

    constexpr GameResult next(BoardMove move) noexcept {
        // board modification - adding token position
        underlying_type chosedColumnCover = (cover >> (singleColumnCoverBits * move.position.col)) & columnCoverMask;
        underlying_type movePosition = static_cast<underlying_type>(std::to_underlying(move.player)) << (chosedColumnCover * cols + move.position.col);
        board |= movePosition;

        // cover modification - adjustment of a new token position
        cover += static_cast<underlying_type>(1u) << (singleColumnCoverBits * move.position.col);

        // check result
        return checkGameResult(move, chosedColumnCover);
    }


private:

    constexpr GameResult checkGameResult(const BoardMove lastMove, underlying_type chosenRow) const noexcept {
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

        auto getWinner = [&]{
            return lastMove.player == Player::player1 ? GameResult::player1_wins : GameResult::player2_wins;
        };

        auto& pos = lastMove.position;

        const auto minRow = chosenRow >= 3u ? chosenRow - 3 : 0u;
        [[maybe_unused]] const auto maxRow = std::min(chosenRow + 3Lu, rows);
        [[maybe_unused]] const auto minCol = pos.col >= 3u ? pos.col - 3 : 0u;
        [[maybe_unused]] const auto maxCol = std::min(pos.col + 3Lu, rows);

        {
            // 1) vertical positions
            if (chosenRow >= 3) {  // start checking from 4th row
                underlying_type b = board;
                b >>= minRow * cols + pos.col;
                static constexpr underlying_type tokenMask = 1u;
                const auto player = std::to_underlying(lastMove.player);
                if ((b & tokenMask) == player and
                    ((b >> cols) & tokenMask) == player and
                    ((b >> (cols * 2)) & tokenMask) == player and
                    ((b >> (cols * 3)) & tokenMask) == player) {
                    return getWinner();
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

    static constexpr size_t singleColumnCoverBits = std::bit_width(rows+1);
    static constexpr size_t columnCoverBits = singleColumnCoverBits * cols;

    static constexpr underlying_type columnCoverMask = [] () {
        underlying_type mask = 1;
        for (unsigned int i = 0; i < singleColumnCoverBits - 1; ++i) {
            mask <<= 1;
            mask |= 1;
        }
        return mask;
    }();


    underlying_type board : size;
    underlying_type cover : columnCoverBits;
};




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

        if (not currentState.validate(move))
            return std::unexpected("Invalid move!");

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
