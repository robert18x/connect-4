#pragma once

#include "utilities/int_types.h"

#include <beman/inplace_vector/inplace_vector.hpp>

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <vector>
#include <cstdint>
#include <utility>

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


enum class GameResult : std::uint8_t {
    player1_wins = std::to_underlying(Player::player1),
    player2_wins = std::to_underlying(Player::player2),
    none,
    draw,
};


template <Cols _cols, Rows _rows>
class Board {
public:
    explicit constexpr Board() = default;
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
        using underlying_type = uint_type<cols>;
        underlying_type col;
    };

    constexpr std::optional<Player> checkAt(Cols col, Rows row) const noexcept {
        if (col > cols or row > rows) {
            return std::nullopt;
        }
        underlying_type columnFill = getColumnCover(col.n);
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

    constexpr std::vector<Position> getAvailableMovePositions() const {
        std::vector<Position> availableMovePositions;
        underlying_type c = cover;
        for (typename Position::underlying_type i = 0u; i < cols; ++i) {
            underlying_type columnCover = c & columnCoverMask;
            if (columnCover < rows) { // check if not full column
                availableMovePositions.push_back(Position{.col = i});
            }
            c >>= singleColumnCoverBits;
        }
        return availableMovePositions;
    }

    constexpr std::size_t getNumberOfOccupiedTokens() const noexcept {
        std::size_t nTokens = 0;
        underlying_type c = cover;
        while (c) {
            underlying_type columnCover = c & columnCoverMask;
            nTokens += columnCover;
            c >>= singleColumnCoverBits;
        }
        return nTokens;
    }

    constexpr bool isFull() const noexcept {
        static constexpr underlying_type fullColumnCover = []() consteval {
            underlying_type fullCover = rows;
            for (unsigned int i = 1u; i < cols; ++i) {
                fullCover <<= singleColumnCoverBits;
                fullCover |= rows;
            }
            return fullCover;
        }();

        return cover == fullColumnCover;
    }

    constexpr bool validate(Position position) const noexcept {
        return position.col < cols and getColumnCover(position.col) < rows;
    }

    constexpr GameResult next(BoardMove move) noexcept {
        // board modification - adding token position
        underlying_type chosenColumnCover = getColumnCover(move.position.col);
        underlying_type movePosition = static_cast<underlying_type>(std::to_underlying(move.player)) << (chosenColumnCover * cols + move.position.col);
        board |= movePosition;

        // cover modification - adjustment of a new token position
        cover += static_cast<underlying_type>(1u) << (singleColumnCoverBits * move.position.col);

        // check result
        return checkGameResult(move, chosenColumnCover);
    }


private:
    constexpr inline underlying_type getColumnCover(std::integral auto col) const noexcept {
        return (cover >> (singleColumnCoverBits * col)) & columnCoverMask;
    }

    constexpr GameResult checkGameResult(const BoardMove lastMove, underlying_type moveRow) const noexcept {
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

        auto& col = lastMove.position.col;

        const auto minRow = moveRow >= 3u ? moveRow - 3 : 0u;
        const auto maxRow = std::min(moveRow + 3Lu, rows);
        const auto minCol = col >= 3u ? col - 3 : 0u;
        const auto maxCol = std::min<size_t>(col + 3Lu, rows);

        static constexpr underlying_type tokenMask = 1u;
        const uint8_t player = std::to_underlying(lastMove.player);

        {
            // 1) vertical positions
            if (moveRow >= 3) {  // start checking from 4th row
                underlying_type b = board;
                b >>= minRow * cols + col;
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
            uint8_t tokens = 1u;
            // * upper left
            if (col > 0) {
                for (uint8_t c = col-1, r = moveRow+1; c >= minCol and r <= maxRow and tokens < 4u; --c, ++r) {
                    underlying_type chosenColumnCover = getColumnCover(c);
                    if (chosenColumnCover <= r) {
                        break;
                    }
                    underlying_type position = (board >> (r * cols + c)) & tokenMask;
                    if (position == player) {
                        ++tokens;
                    } else {
                        break;
                    }
                }
            }

            // * bottom right
            if (moveRow > 0) {
                for (uint8_t c = col+1, r = moveRow-1; c <= maxCol and r >= minRow and tokens < 4u; ++c, --r) {
                    underlying_type chosenColumnCover = getColumnCover(c);
                    if (chosenColumnCover <= r) {
                        break;
                    }
                    underlying_type position = (board >> (r * cols + c)) & tokenMask;
                    if (position == player) {
                        ++tokens;
                    } else {
                        break;
                    }
                }
            }

            if (tokens >= 4) {
                return getWinner();
            }
        }

        {
            // 3) horizontal: left to rignt
            uint8_t tokens = 1u;

            // * left
            if (col > 0) {
                for (uint8_t c = col-1; c >= minCol and tokens < 4u; --c) {
                    underlying_type chosenColumnCover = getColumnCover(c);
                    if (chosenColumnCover <= moveRow) {
                        break;
                    }
                    underlying_type position = (board >> (moveRow * cols + c)) & tokenMask;
                    if (position == player) {
                        ++tokens;
                    } else {
                        break;
                    }
                }
            }

            // * right
            for (uint8_t c = col+1; c <= maxCol and tokens < 4u; ++c) {
                underlying_type chosenColumnCover = getColumnCover(c);
                if (chosenColumnCover <= moveRow) {
                    break;
                }
                underlying_type position = (board >> (moveRow * cols + c)) & tokenMask;
                if (position == player) {
                    ++tokens;
                } else {
                    break;
                }
            }

            if (tokens >= 4) {
                return getWinner();
            }
        }

        {
            // 4) bottom left to upper right positions
            uint8_t tokens = 1u;

            // * bottom left
            if (moveRow > 0 and col > 0) {
                for (uint8_t c = col-1, r = moveRow-1; c >= minCol and r >= minRow and tokens < 4u; --c, --r) {
                    underlying_type chosenColumnCover = getColumnCover(c);
                    if (chosenColumnCover <= r) {
                        break;
                    }
                    underlying_type position = (board >> (r * cols + c)) & tokenMask;
                    if (position == player) {
                        ++tokens;
                    } else {
                        break;
                    }
                }
            }

            // * upper right
            for (uint8_t c = col+1, r = moveRow+1; c <= maxCol and r <= maxRow and tokens < 4u; ++c, ++r) {
                underlying_type chosenColumnCover = getColumnCover(c);
                if (chosenColumnCover <= r) {
                    break;
                }
                underlying_type position = (board >> (r * cols + c)) & tokenMask;
                if (position == player) {
                    ++tokens;
                } else {
                    break;
                }
            }

            if (tokens >= 4) {
                return getWinner();
            }
        }

        if (isFull()) {
            return GameResult::draw;
        } else {
            return GameResult::none;
        }
    }

    static constexpr size_t singleColumnCoverBits = std::bit_width(rows+1);
    static constexpr size_t columnCoverBits = singleColumnCoverBits * cols;
    static constexpr underlying_type columnCoverMask = [] () consteval {
            underlying_type mask = 1;
            for (unsigned int i = 0; i < singleColumnCoverBits - 1; ++i) {
                mask <<= 1;
                mask |= 1;
            }
            return mask;
        }();

    underlying_type board : size = {0};
    underlying_type cover : columnCoverBits = {0};
};
