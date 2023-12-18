#include "bot.h"
#include <stdio.h>

// TODO hard code first move (too slow)

typedef struct {
    int score;
    Coords move;
} BotMove;

BotMove minimax(Board board, Cell side) {
    if (is_finished(board)) {
        Cell winner = winner_of(board).side;
        return (BotMove) {
            (winner == EMPTY ? 0 : 1) * (winner == side ? 1 : -1),
            NONE
        };
    }

    // Default init value, will always be overwritten by first possible move
    BotMove best_move = { -2, NONE };

    for (Coords move = TOP_LEFT; move <= BOTTOM_RIGHT; move++) {
        if (cell_at(board, move) == EMPTY) {
            Board test_board = placed_cell(board, side, move);
            int score = -minimax(test_board, OTHER_SIDE(side)).score;
            printf("score %d\n", score);
            if (score > best_move.score) {
                best_move = (BotMove) { score, move };
            }
        }
    }

    return best_move;
}

Board bot_placed_cell(Board board) {
    // Assumes that bot can play (game not yet finished)
    BotMove move = minimax(board, BOT_SIDE);

    return placed_cell(board, BOT_SIDE, move.move);
}
