using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XNATetris
{
    class TetrisShape : TetrisConstants
    {
        public bool isGameOver = false;
        public bool isInited = false;

        private int mOrient;
        private int mType;
        private int mNextType;
        public int getNextType() { return mNextType; }
        private int mState;
        private int[] mElems;

        private Random mRand;
        private TetrisGrid mGrid;


        public TetrisShape(TetrisGrid g)
        {
            mGrid = g;
            mRand = new Random();
            mElems = new int[MAX_ELEMS];
            mNextType = mRand.Next(TYPE_MAX_TYPES);
        }

        public bool spawn()
        {
            isGameOver = false;
            for (int i = 0; i < mElems.Length; i++)
            {
                mElems[i] = DONT_CHECK_CELL;
            }
            mType = mNextType;
            mNextType = mRand.Next(TYPE_MAX_TYPES);
            mElems[ELEM_BASE] = START_CELL;
            mOrient = START_ORIENTATION;
            mState = STATE_USER;
            return alignFromOrientation(mOrient);
        }

        public void update(int currentAction)
        {
            if (mState == STATE_USER)
            {
                switch (currentAction)
                {
                    case ACTION_STRAFE_LEFT:
                        {
                            shiftCells(C_LEFT);
                            break;
                        }
                    case ACTION_STRAFE_RIGHT:
                        {
                            shiftCells(C_RIGHT);
                            break;
                        }
                    case ACTION_ROTATE_L:
                        {
                            if (rotateShape(ROT_LEFT))
                                TetrisGame.soundPlayer.FireSoundFX(SOUND_FX.Rotate);
                            break;
                        }
                    case ACTION_ROTATE_R:
                        {
                            if (rotateShape(ROT_RIGHT))
                                TetrisGame.soundPlayer.FireSoundFX(SOUND_FX.Rotate);
                            break;
                        }
                    case ACTION_MAKE_FALL:
                        {
                            mState = STATE_FALLING;
                            TetrisGame.soundPlayer.FireSoundFX(SOUND_FX.Fall);
                            break;
                        }
                }
            }
        }

        public bool IsFalling()
        {
            return mState == STATE_FALLING;
        }

        //true when need to recheck playfield
        public bool addGravity()
        {
            if (isInited)
            {
                //gravity
                bool falling = shiftCells(C_DOWN);
                if (!falling)
                {
                    mState = STATE_LOCKED;
                    isInited = false;
                    return true;
                }
            }
            else
            {
                isInited = spawn();

                if (!isInited)//this means no room to init = game over
                {
                    isGameOver = true;
                }
            }
            return false;
        }

        private bool rotateShape(int rotation)
        {
            int orient = mOrient;
            orient += rotation;
            if (orient < OR_NORTH)
                orient = OR_WEST;
            else if (orient > OR_WEST)
                orient = OR_NORTH;
            bool hasRotated = alignFromOrientation(orient);
            if (hasRotated)
                mOrient = orient;

            return hasRotated;
        }


        private bool alignFromOrientation(int orientation)
        {
            int[] newElemPos = (int[])mElems.Clone();
            int typeOffset = (mType * SHAPE_TABLE_TYPE_OFFSET);
            orientation *= SHAPE_TABLE_ELEMS_PER_ROW;
            newElemPos[ELEM_1] = newElemPos[ELEM_BASE] + SHAPE_TABLE[typeOffset + orientation + SHAPE_TABLE_ELEMS_1];
            newElemPos[ELEM_2] = newElemPos[ELEM_BASE] + SHAPE_TABLE[typeOffset + orientation + SHAPE_TABLE_ELEMS_2];
            newElemPos[ELEM_3] = newElemPos[ELEM_BASE] + SHAPE_TABLE[typeOffset + orientation + SHAPE_TABLE_ELEMS_3];

            return tryToMove(newElemPos);
        }


        private bool shiftCells(int cellscrool)
        {
            int[] tmpElems = (int[])mElems.Clone();
            for (int i = 0; i < mElems.Length; i++)
            {
                if (mElems[i] != DONT_CHECK_CELL)
                    tmpElems[i] += cellscrool;
            }

            if (cellscrool == C_LEFT || cellscrool == C_RIGHT)
                if (mGrid.getRow(mElems[ELEM_BASE]) != mGrid.getRow(tmpElems[ELEM_BASE]))
                    return false;

            return tryToMove(tmpElems);
        }

        private bool tryToMove(int[] newElemPos)
        {
            bool hasMoved = mGrid.tryToMoveCells(mElems, newElemPos);
            if (hasMoved)
                mElems = newElemPos;
            return hasMoved;
        }

    }
}
