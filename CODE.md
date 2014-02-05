A quick guide to OpenStrike code.

Most of the game code is split into topic directories, each then
built into a static library which are linked with the utilities and
the game itself.

* ```lib/dat``` - contains classes which handle .DAT file contents
  * ```lib/dat/buffer.*``` - low level memory buffer handling, decoding and slicing.
  * ```lib/dat/datfile.*``` - .DAT file reader. Reads .DAT file toc, can enumerate its entries and return data for requested entry. As the data is compressed in .DAT file, it uses unpacker from the next entry
  * ```lib/dat/unpacker.*``` - unpacker for compression used in .DAT file
  * ```lib/dat/datgraphics.*``` - handler for specific type of .DAT file content, graphic files. Enumerates individial sprites inside these, returns their properties and pixels data itself
* ```lib/graphics``` - game painting code
  * ```lib/graphics/spritemanager.*``` - a manager which packs separate small sprites onto larger textures, and provides methods to paint these sprites
  * ```lib/graphics/rectpacker.*``` - rectangle packer used in sprite manager
  * ```lib/graphics/renderer.*``` - renderer for all game objects
* ```lib/game``` - game logic
  * ```lib/gameobject.*``` - base of all game objects (player, enemy units etc.)
  * ```lib/visitor.*``` - base class for visitor pattern which knows how to process each kind of game objects used, for example, to separate rendering logic from object logic. Used as base for renderer, for instance
  * ```lib/game.*``` - main game class which holds all objects in the current game and provides processing and interaction for them
* ```lib/gameobjects``` - logic of all game objects
