#pragma once

template <typename _Board>
class Bot {
public:
    explicit constexpr Bot() = default;

    using Board = _Board;
    using Move = typename Board::BoardMove;


};
