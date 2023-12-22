#include "bot.h"

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
            if (score > best_move.score) {
                best_move = (BotMove) { score, move };
            }
        }
    }

    return best_move;
}

Board bot_placed_cell(Board board) {
    // Assumes that bot can play (game not yet finished)

    if (is_second_move(board)) {
        Coords best_moves[] = {
            MID_MID, TOP_LEFT, MID_MID,
            TOP_LEFT, TOP_LEFT, TOP_RIGHT,
            MID_MID, TOP_MID, MID_MID,
        };
        return placed_cell(board, BOT_SIDE, best_moves[first_move_coords(board)]);
    }

    Coords move = minimax(board, BOT_SIDE).move;

    return placed_cell(board, BOT_SIDE, move);
}
