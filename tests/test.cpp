#include <snitch/snitch.hpp>
#include "game/Game.h"


TEST_CASE("Game model test", "[test player move]" ) {
    using Connect4Board = Board<Cols(7), Rows(6)>;

    CONSTEXPR_REQUIRE([]() constexpr {
        Game<Connect4Board> game;
        return game.makeMove(Move<Connect4Board>());
    }() == GameResult::none);


}
