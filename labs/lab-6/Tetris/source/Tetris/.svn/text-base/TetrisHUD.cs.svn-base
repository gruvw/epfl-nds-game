using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace XNATetris
{
    class TetrisHud : TetrisConstants
    {

        public static void paintRightHud(int right, int top, int score, int nextType)
        {
            int x, y;

            //score
            x = right - HUD_SCORE_TEXT_OFFSET;
            y = top + MARGIN_TOP + HUD_SCORE_Y_START;
            SpecificTools.QuickDrawString("Score", x, y, TetrisGame.FontSmall, HUD_SCORE_WORD_COLOR, SpecificTools.Align.Right, true, Color.Black);
            y += HUD_SCORE_INTERLINE;
            SpecificTools.QuickDrawString("" + score, x, y, TetrisGame.FontSmall, HUD_SCORE_NUM_COLOR, SpecificTools.Align.Right);

            //next shape
            displayNextShape(right, top, nextType);

            //top scores
            displayTopScores(right, top);

        }


        public static void displayNextShape(int startX, int startY, int nextType)
        {
            int tmpX, tmpY, offset;
            int i = 0;
            int x = startX - HUD_NEXT_TEXT_OFFSET;
            int y = startY + MARGIN_TOP + HUD_NEXT_WORD_Y_START;

            SpecificTools.QuickDrawString("Next", x, y, TetrisGame.FontSmall, HUD_NEXT_WORD_COLOR, SpecificTools.Align.Right, true, Color.Black);
            x = startX - HUD_NEXT_SHAPE_X_START;
            y = startY + MARGIN_TOP + HUD_NEXT_SHAPE_Y_START;
            tmpX = x;
            tmpY = y;
            offset = (nextType * SHAPE_TABLE_TYPE_OFFSET) + START_ORIENTATION * SHAPE_TABLE_ELEMS_PER_ROW;
            do
            {
                TetrisGame.ShapeManager.DrawRect(TetrisGame.Pixel, TetrisGame.spriteBatch, new Rectangle(tmpX, tmpY, HUD_NEXT_SHAPE_CELL_SIZE, HUD_NEXT_SHAPE_CELL_SIZE), HUD_NEXT_SHAPE_COLOR);
                switch (SHAPE_TABLE[offset + i])
                {
                    case C_LEFT:
                        tmpX = x - HUD_NEXT_SHAPE_CELL_OFFSET;
                        tmpY = y;
                        break;
                    case C_RIGHT:
                        tmpX = x + HUD_NEXT_SHAPE_CELL_OFFSET;
                        tmpY = y;
                        break;
                    case C_UP:
                        tmpX = x;
                        tmpY = y - HUD_NEXT_SHAPE_CELL_OFFSET;
                        break;
                    case C_DOWN:
                        tmpX = x;
                        tmpY = y + HUD_NEXT_SHAPE_CELL_OFFSET;
                        break;
                    case C_LEFT + C_DOWN:
                        tmpX = x - HUD_NEXT_SHAPE_CELL_OFFSET;
                        tmpY = y + HUD_NEXT_SHAPE_CELL_OFFSET;
                        break;
                    case C_RIGHT + C_DOWN:
                        tmpX = x + HUD_NEXT_SHAPE_CELL_OFFSET;
                        tmpY = y + HUD_NEXT_SHAPE_CELL_OFFSET;
                        break;
                    case C_LEFT + C_UP:
                        tmpX = x - HUD_NEXT_SHAPE_CELL_OFFSET;
                        tmpY = y - HUD_NEXT_SHAPE_CELL_OFFSET;
                        break;
                    case C_RIGHT + C_UP:
                        tmpX = x + HUD_NEXT_SHAPE_CELL_OFFSET;
                        tmpY = y - HUD_NEXT_SHAPE_CELL_OFFSET;
                        break;
                    case C_RIGHT * 2:
                        tmpX = x - (HUD_NEXT_SHAPE_CELL_OFFSET * 2);//i am cheating here and moving to the left for better hud display (anchor is on right)
                        tmpY = y;
                        break;
                    default:
                        //need to manage
                        break;
                }
                i++;
            } while (i < MAX_ELEMS);
        }

        public static void displayTopScores(int x, int y)
        {

            x -= HUD_TOP_SCORES_TEXT_OFFSET;
            y += MARGIN_TOP + HUD_TOP_SCORES_Y_START;

            //paint title
            SpecificTools.QuickDrawString("TopScore", x, y, TetrisGame.FontSmall, HUD_TOP_SCORES_TITLE_COLOR, SpecificTools.Align.Right, true, Color.Black);
            y += 30;

            for (int i = 0; i < TetrisGame.highScore.CurrentTopScores.Count; i++, y += 24)
            {
                String name = TetrisGame.highScore.CurrentTopScores.PlayerName[i];
                int score = TetrisGame.highScore.CurrentTopScores.Score[i];
                if(score > 0)
                    SpecificTools.QuickDrawString((i + 1) + ". " + name + ": " + score, x, y, TetrisGame.FontSmall, HUD_TOP_SCORES_RANK_COLOR, SpecificTools.Align.Right);
                else
                    SpecificTools.QuickDrawString("No Score yet", x, y, TetrisGame.FontSmall, HUD_TOP_SCORES_RANK_COLOR, SpecificTools.Align.Right);

            }
        }
    }
}
