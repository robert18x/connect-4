#include "game/Game.h"
#include <print>


int main() {
    Board<Cols(6), Rows(3)> board;
    std::println("Board struct size: {}", sizeof(board));
}
