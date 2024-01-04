# Tic-Tac-Tile NDS Game Project

EPFL BA5 EE-310 NDS project  
Lucas Jung (324724)

This game is a [Tic-Tac-Toe](https://en.wikipedia.org/wiki/Tic-tac-toe) clone on the [NDS](https://en.wikipedia.org/wiki/Nintendo_DS).

**Name**: the name Tic-Tac-Tile is a modification of the original Tic-Tac-Toe game name.
It is a reference to the word "TacTile", denoting the bottom tactile screen of the NDS.
Additionally, the last word "Tile" is a reference to the tile mode of the graphical engines of the NDS.
The last layer of meaning behind the name is the sound of a clock "tic tac" as there is some kind of a time pressure aspect to the game.

## Showcase

Here are a few pictures of the Tic-Tac-Tile game:

<div>
  <img src="https://github.com/gruvw/epfl-nds-game/blob/main/project/tic-tac-tile/docs/nds.jpg" alt="NDS" width="600px">
  <br>
  <img src="https://github.com/gruvw/epfl-nds-game/blob/main/project/tic-tac-tile/docs/menu.png" alt="Game Menu" width="197px"> 
  <img src="https://github.com/gruvw/epfl-nds-game/blob/main/project/tic-tac-tile/docs/game.png" alt="Game Play" width="197px"> 
  <img src="https://github.com/gruvw/epfl-nds-game/blob/main/project/tic-tac-tile/docs/over.png" alt="Game Over" width="197px"> 
</div>

## Instructions

Instructions to play Tic-Tac-Tile :)

### Installation

To install the game, you will need to obtain the `tic-tac-tile.nds` game file.
Use one of the following method:

- Download it online from the GitHub releases: [`tic-tac-tile.nds`](https://) <!-- TODO -->
- Build it from source by running `make build` in the same directory as this README (requires [`devkitpro-pacman`](https://apt.devkitpro.org/install-devkitpro-pacman)).

Place the `tic-tac-tile.nds` file inside the micro SD card of an [R4 cartridge](https://en.wikipedia.org/wiki/R4_cartridge).

### Game

Once the game boots up, you'll be welcomed by a nice background music and the settings menu.

#### Settings

It features 3 **game modes**:

* Single player mode, against bot
* Two player mode, single NDS
* Two player mode, two NDS over Wi-Fi

Select among the 3 **speeds**:

* Slow (green)
* Medium (orange)
* Fast (red)

#### Play

1. Select the settings that you want by using the bottom touch screen.
2. Press on the `start` button to launch a game.
    * In Wi-Fi two player mode, <!-- TODO wifi explain in wifi section, do not press start at same time, press start while search to reinitiate connection process, if Wi-Fi connection is totally lost no way to know so reboot NDS, if momentary lost it's ok they will sync packets -->
3. Select the square where you want to place your piece using the `Left`, `Right`, `Up`, `Down` arrow buttons.
4. Press on `A` to confirm your choice.
   Be carefull about the progress bar, you need to play before you run out of time to win!
5. Wait for the other side to play.
    * In single player mode, the bot will play instantly after your turn.
    * In (local) two player, mode you need to pass the Nintendo DS to the other player so they can play.
    * In Wi-Fi two player mode, simply wait for your opponent to play on their Nintendo DS.
6. Carry on till you reach the Game Over screen. Alternatively you can also press `start` at any moment to put an end to the current game and go directly to the Game Over screen.
    * If one of the side won, their 3 winning pieces will be highlighted on the board.
      Their piece will be crowned on the bottom screen.
    * A clock will be displayed on the bottom screen in case they won because the other side ran out of time.
    * No additional information will be displayed if the game resulted in a draw.
      You will simply see the Game Over screen and the final state of the board.
7. You can start a new game by pressing the `start` button to go back to the settings selection menu.

Have fun !

<!-- TODO technicals section, P2P-BOP + ACK fsm -->
<!-- TODO document key SELECT (reset and reinitiate wifi connection process) -->
<!-- TODO WIFI is possible to play multiple simultaneous games of tic-tac-tile on same network and also multiple other NDS wifi games should not interfere -->
<!-- TODO WIFI stack do not track disconnection from peer, once it is shown as connected it will stay in this state, if the peer is disconnected before successfully (acked) communicating the end of the game, the local device will need to end the game them self (press start) or if game is not started yet press SELECT to restart connection establishment process -->

## Roadmap

- [X] Board model + utilities
- [X] Main background graphics
- [X] Connect board model to view
- [X] Keys handling + cell selection
- [X] Sub background settings graphics
- [X] Settings selection touch
- [X] Begin and game over screens
- [X] Custom palettes generation
- [X] AI (minimax)
- [X] Timer + progress bar
- [X] Game Over sprites
- [X] Improve project structure
- [X] Music + sound effects
- [X] Polish code + add comments
- [X] Fully test game
- [X] Presentation
- [X] Write README
- [ ] Wi-Fi two player mode
- [ ] Clean, comment, and polish code
- [ ] README code structure + BOP protocol explain
- [ ] Update presentation
- [ ] Fully test game
- [ ] GitHub release (add to README)
