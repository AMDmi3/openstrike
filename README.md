# OpenStrike

F/OSS Desert Strike and Jungle Strike reimplementation.

## Status

The project is currently on the early stages of development, so the
only functionality exists at the time is a set of classes to read
.DAT file format from Desert and Jungle Strike games and decode
sprites from it. On top of that, two utilities exists: unpacker
which extracts separate files stored in .DAT and viewer which is
interactive SDL2 application used to view sprite sets.

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

After the project is built, you can run the utilities. Both require
path to .DAT file from Desert or Jungle Strike (usually named
DESERT.DAT or JUNGLE.DAT correspondingly), further referred to as
```file.DAT```

* View contents of data file (as a list of entries):
  ```
util/unpacker/unpacker -l file.DAT
```

* Extract contents of data file (into current directory):
  ```
util/unpacker/unpacker -x file.DAT
```

* Interactively view sprites from a data file:
  ```
util/viewer/viewer file.DAT
```

in this utility, use arrow keys or Page Up/Page Down to navigate
through graphics and Q or Escape to close the viewer.

## Author

* [Dmitry Marakasov](https://github.com/AMDmi3) <amdmi3@amdmi3.ru>

## License

GPLv3, see COPYING
