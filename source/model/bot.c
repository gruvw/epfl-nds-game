#include "bot.h"

typedef struct {
    int score;
    Coords move;
} BotMove;

// Will compute the best move possible on the `board` for `side`
BotMove minimax(Board board, Cell side) {
    if (is_board_finished(board)) {
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
            if (score > best_move.score) {
                best_move = (BotMove) { score, move };
            }
        }
    }

    return best_move;
}

// Plays the best move on the `board` for the `BOT_SIDE`
Board bot_placed_cell(Board board) {
    // Assumes that bot can play: game not finished yet

    if (is_second_move(board)) {
        // Hard coded cache of first bot move (second total move) as it is too slow (~2 seconds) to compute
        Coords best_moves[] = {  // cannot be global const, "not compile time constant"...
            MID_MID, TOP_LEFT, MID_MID,
            TOP_LEFT, TOP_LEFT, TOP_RIGHT,
            MID_MID, TOP_MID, MID_MID,
        };
        return placed_cell(board, BOT_SIDE, best_moves[first_move_coords(board)]);
    }

    Coords move = minimax(board, BOT_SIDE).move;

    return placed_cell(board, BOT_SIDE, move);
}
