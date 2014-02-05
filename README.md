# OpenStrike

F/OSS Desert Strike and Jungle Strike reimplementation.

## Status

The project is currently on the early stages of development, and
is not yet playable. What it can offer right now:

* An utility to unpack Desert/Jungle Strike .DAT files
* SDL2 - based viewer for sprites in these files
* A game skeleton which currently demonstrates an ability to load
  and render some sprites

## Building

Dependencies:

* cmake
* SDL2

The project also uses libSDL2pp, C++11 bindings library for SDL2.
It's included into git repository as a submodule, so if you've
obtained source through git, don't forget to run ```git submodule
init && git submodule update```.

To build the project, run:

```cmake . && make```

## Running

To run any of binaries produced by the project, you need original
Desert/Jungle Strike .DAT files (further referred as ```file.DAT```)

### Unpacker

To view all entries in .DAT file:
```
% util/unpacker/unpacker -l file.DAT
```

To extract them into current directory:
```
% util/unpacker/unpacker -x file.DAT
```

### Viewer

```
util/viewer/viewer file.DAT
```

Use arrow keys or Page Up/Page Down to navigate through graphics
and Q or Escape to close the viewer.

### Game

```
src/openstrike file.DAT
```

Use left/right keys to rotate the chopper. That's all it does for
now.

## Author

* [Dmitry Marakasov](https://github.com/AMDmi3) <amdmi3@amdmi3.ru>

## License

GPLv3, see COPYING

The project also bundles third party software under its own licenses:

* extlibs/SDL2pp (C++11 SDL2 wrapper library) - zlib license
* extlibs/boost (a tiny bit from boost) - Boost Software License
