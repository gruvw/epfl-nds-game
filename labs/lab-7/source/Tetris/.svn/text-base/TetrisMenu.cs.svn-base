using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace XNATetris
{
    class TetrisMenu
    {
        
        public enum MENU_STATE
        {
            STATE_MENUOFF,
            STATE_MENUSTART,
            STATE_MENURESUME,
            STATE_MENUGAMEOVER,
            STATE_ENTERNAME,
        }

        private MENU_STATE status;
        private int currentSelected;
        private int currentAvailableOptions;
        private int currentLateralSelected;
        private int currentLateralAvailableOptions;
        private bool allowcycle;
        private String[] currentOptions;
        private byte[] player;
        private Color bkgColor;
        private Color wordColor;
        private Color wordColorSelected;
        private bool isHighScore;

        public TetrisMenu()
        {
            player = new byte[3];
            bkgColor = new Color(192, 192, 192, 216);
            wordColor = Color.Black;
            wordColorSelected = Color.Green;
            setMenuState(MENU_STATE.STATE_MENUSTART);
        }

        public void setMenuState(MENU_STATE state, bool highScore)
        {
            setMenuState(state);
            isHighScore = highScore;//MUST be called after set state because set state sets it to default
        }

        public void setMenuState( MENU_STATE state )
        {
            isHighScore = false;
            status = state;
            currentSelected = 0; //always reset this
            currentLateralSelected = 0;
            currentLateralAvailableOptions = 0;//default
            allowcycle = false;//default
            switch (status)
            {
                case MENU_STATE.STATE_MENURESUME:
                    currentOptions = MENU_RESUME_STRINGS;
                    currentAvailableOptions = currentOptions.Length;
                    break;
                case MENU_STATE.STATE_MENUGAMEOVER:
                    currentOptions = MENU_RESTART_STRINGS;
                    currentAvailableOptions = currentOptions.Length;
                    break;
                case MENU_STATE.STATE_MENUSTART:
                    currentOptions = MENU_START_STRINGS;
                    currentAvailableOptions = currentOptions.Length;
                    break;
                case MENU_STATE.STATE_ENTERNAME:
                    currentOptions = EMPTY_STRINGS;
                    currentAvailableOptions = 26;//alphabet!
                    allowcycle = true;
                    currentLateralAvailableOptions = 4; //three letters + ok
                    for (int i = 0; i < player.Length; i++)
                        player[i] = 0;
                    break;
            }
        }

        public bool IsMenuOff()
        {
            return status == MENU_STATE.STATE_MENUOFF;
        }

        public void update_menu(Platform.Input.ACTION a)
        {
            switch (a)
            {
                case Platform.Input.ACTION.RIGHT:
                    if (currentLateralSelected < currentLateralAvailableOptions)
                        currentLateralSelected++;
                    break;
                case Platform.Input.ACTION.LEFT:
                    if (currentLateralSelected > 0)
                        currentLateralSelected--;
                    break;
                case Platform.Input.ACTION.DOWN:
                    if (currentSelected < currentAvailableOptions - 1)
                        currentSelected++;
                    else if (allowcycle)
                        currentSelected = 0;
                    break;
                case Platform.Input.ACTION.UP:
                    if (currentSelected > 0)
                        currentSelected--;
                    else if (allowcycle)
                        currentSelected = currentAvailableOptions-1;
                    break;
                case Platform.Input.ACTION.SELECT:

                    TetrisGame.soundPlayer.FireSoundFX(SOUND_FX.Fall);

                    if (currentOptions.Length == 0)
                    {
                        if (currentLateralSelected == 3)
                        {
                            StringBuilder builder = new StringBuilder();
                            for (int i = 0; i < 3; i++)
                            {
                                builder.Append(((char)('A' + player[i])));
                            }
                            TetrisGame.SetPlayerName(builder.ToString());
                            setMenuState(MENU_STATE.STATE_MENUOFF);
                        }
                        else
                        {
                            update_menu(Platform.Input.ACTION.DOWN);//considering enter on leters is down
                            return;
                        }
                    }
                    else if (currentOptions[currentSelected] == commandStart)
                    {
                        setMenuState(MENU_STATE.STATE_ENTERNAME);
                    }
                    else if (currentOptions[currentSelected] == commandResume)
                    {
                        setMenuState(MENU_STATE.STATE_MENUOFF);
                    }
                    else if (currentOptions[currentSelected] == commandQuit)
                    {
                        TetrisGame.ExitGame();
                    }
                    else if (currentOptions[currentSelected] == commandRestart)
                    {
                        setMenuState(MENU_STATE.STATE_MENUOFF);
                        TetrisGame.RestartGame();
                    }
                    break;
            }

            //specific for player name menu
            if (status == MENU_STATE.STATE_ENTERNAME && currentLateralSelected < 3)
            {
                if(a==XNATetris.Platform.Input.ACTION.DOWN || a == XNATetris.Platform.Input.ACTION.UP)
                {
                    player[currentLateralSelected] = (byte)currentSelected;
                    TetrisGame.soundPlayer.FireSoundFX(SOUND_FX.Rotate);
                }
                else if (a == XNATetris.Platform.Input.ACTION.LEFT || a == XNATetris.Platform.Input.ACTION.RIGHT)
                        currentSelected = player[currentLateralSelected];
            }
        }


        public void paint()
        {
            int w, h;
            if (status != MENU_STATE.STATE_MENUOFF)
            {
                w = TetrisGame.graphics.PreferredBackBufferWidth;
                h = TetrisGame.graphics.PreferredBackBufferHeight;
                TetrisGame.spriteBatch.Draw(TetrisGame.Pixel, new Rectangle(0, 0, w, h), bkgColor);
                SpecificTools.QuickDrawString("XNA Tetris by Chris FERRARIO @ chris-ferrario.com", 20, 20, TetrisGame.FontSmall, Color.Blue, SpecificTools.Align.Left, true, Color.White);
                paintMenuStrings(currentOptions, w, h);
                paintSpecific(w,h);
            }
        }

        private void paintMenuStrings(String[] options, int screenW, int screenH)
        {
            screenH /= options.Length + 1;
            int y = screenH;
            for (int i = 0; i < options.Length; i++, y += screenH)
            {
                SpecificTools.QuickDrawString(options[i], screenW / 2, y, TetrisGame.FontBig, i == currentSelected ? wordColorSelected : wordColor, SpecificTools.Align.Center, true, Color.White);
            }
        }

        private void paintSpecific(int screenW, int screenH)
        {
            if(status == MENU_STATE.STATE_MENUGAMEOVER)
            {
                String message = isHighScore ? "Congratulations you have reached a high score!" : "Sorry you have lost without achieving a high score";
                SpecificTools.QuickDrawString(message, screenW / 2, 80, TetrisGame.FontSmall, Color.Red, SpecificTools.Align.Center, true, Color.White);
            }
            else if (status == MENU_STATE.STATE_ENTERNAME)
            {
                int interspace = 40;
                int x = screenW / 2;
                int y = 120;
                SpecificTools.QuickDrawString("Enter your desired name:", x, y, TetrisGame.FontSmall, Color.Black, SpecificTools.Align.Center, true, Color.White);

                TetrisGame.spriteBatch.Draw(TetrisGame.Pixel, new Rectangle(x - 100, y + 60, 120, 50), Color.White);
                x -= 2 * interspace;
                y += 60;


                for (int i = 0; i < 3; i++, x+=interspace)
                {
                    SpecificTools.QuickDrawString(((char)('A' + player[i])).ToString(), x, y, TetrisGame.FontBig, i == currentLateralSelected ? Color.Red: Color.Blue, SpecificTools.Align.Center);
                }
                SpecificTools.QuickDrawString("Confirm", x, y + 14, TetrisGame.FontSmall, currentLateralSelected == 3 ? Color.Red : Color.Blue, SpecificTools.Align.Left, true, Color.White);
            }
        }


        //Menu Tables
        private static String commandStart = "Start Game";
        private static String commandResume = "Resume Game";
        private static String commandRestart = "Restart Game";
        private static String commandQuit = "Quit Game";
        private String[] MENU_START_STRINGS =
        {
            commandStart,
            commandQuit,
        };
        private String[] MENU_RESUME_STRINGS =
        {
            commandResume,
            commandQuit,
        };
        private String[] MENU_RESTART_STRINGS =
        {
            commandRestart,
            commandQuit,
        };
        private String[] EMPTY_STRINGS = {};
    }
}
