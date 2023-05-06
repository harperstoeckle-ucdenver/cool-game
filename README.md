## Build
This project uses CMake, so make sure that CMake is installed before attempting to build. Before
this can be built, dependencies must be pulled using the command

```sh
git submodule init && git submodule update
```

### Native PC Build
This will build the project for PC so it can be played in a terminal. The system *must* have
ncurses.

```sh
cmake -B build && cmake --build build
```

### Arduino Build
This will build for an Arduino. The configuration can be done with the command

```sh
cmake -B build -DCOOL_GAME_PLATFORM=arduino
```

This will probably only work on MacOS. On other platforms, the following procedure *may* work, but I
have not tested it:

1. If you already did the configure command, delete the `build` directory that was created by that
   command, since it has been configured incorrectly.
2. Open `CMakeLists.txt` and look for a line starting with `set(ARDUINO_INSTALL_PATH`. Change the
   text in quotes after that to the root of the Arduino install path on your system.
3. Try the configuration command again.

If the configuration was done successfully, the project can be built and uploaded an Arduino using
the command

```sh
SERIAL_PORT='<serial-port>' cmake --build build --target upload-cool-game
```

where `<serial-port>` is the name of the port connected to the Arduino.
