# perfmon-tui

A lightweight tui system performance monitor built in C++ for Linux.

## Features

- Live updating **CPU** and **Memory** usage
- ANSI-based UI with usage bars
- Minimal dependencies (pure C++17, no ncurses)
- Press `q` to quit cleanly

## Preview

![perfmon-tui screenshot](screenshot.png)

## Build Instructions

### Requirements

- C++17 compiler (e.g., `g++`, `clang++`)
- Linux OS with `/proc/` filesystem (e.g., Arch, Ubuntu)

### Build

```bash
make
./perfmon-tui
```
## License
  This project is licensed under the GNU GPLv3
