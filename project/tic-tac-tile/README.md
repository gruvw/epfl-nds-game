# NDS Tic-Tac-Tile Game

EPFL BA5 EE-310 NDS project

This game is a [Tic-Tac-Toe](https://en.wikipedia.org/wiki/Tic-tac-toe) clone on the [NDS](https://en.wikipedia.org/wiki/Nintendo_DS).

## Showcase

Here are a few pictures of the game:

![NDS]()

![Game Menu]()
![Game Play]()
![Game Over]()

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
  * In Wi-Fi two player mode, both players need to press the `start` button to connect the two Nintendos.
3. Select the square where you want to place your piece using the `Left`, `Right`, `Up`, `Down` arrow buttons.
4. Press on `A` to confirm your choice.
   Be carefull about the progress bar, you need to play before you run out of time to win!
5. Wait for the other side to play.
  * In single player mode, the bot will play instantly after your turn.
  * In (local) two player, mode you need to pass the Nintendo DS to the other player so they can play.
  * In Wi-Fi two player mode, simply wait for your opponent to play on their Nintendo DS.
6. Carry on till you reach the Game Over screen.
  * If one of the side won, their 3 winning pieces will be highlighted on the board.
    Their piece will be crowned on the bottom screen.
  * A clock will be displayed on the bottom screen in case they won because the other side ran out of time.
  * No additional information will be displayed if the game resulted in a draw.
    You will simply see the Game Over screen and the final state of the board.
7. You can start a new game by pressing the `start` button to go back to the settings selection menu.

Have fun !

## Roadmap

- [X] Board model + utilities
- [X] Main background graphics
- [X] Connect board model to view
- [X] Keys handling + cell selection
- [X] Sub background settings graphics
- [X] Settings selection touch
- [X] Begin and game over screens
- [X] AI (minimax)
- [X] Timer + progress bar
- [X] Game Over sprites
- [X] Improve project structure
- [X] Music + sound effects
- [X] Polish code + add comments
- [ ] Fully test game
- [ ] Presentation
- [ ] Write README
- [ ] Wi-Fi two player mode
- [ ] Update presentation
