# Arcane Engine

## Overview

Arcane Engine is a modular C++ game engine. This repository contains the source code and build scripts for compiling the engine as a DLL on Windows.

## Prerequisites

- [MinGW-w64](http://mingw-w64.org/) (for `g++` and Windows libraries)
- `make` or `nmake` (for building)
- Development libraries for `libpng`, `dsound`, `user32`, and `gdi32`

## Building

1. Open a terminal in the repository root.
2. Ensure `g++` and required libraries are in your `PATH`.
3. Run:

   ```
   make
   ```

   Or, if using the provided batch script (for MSVC, edit as needed for MinGW):

   ```
   setup.bat
   ```

## Cleaning

To remove build artifacts:

```
make clean
```

## Output

- `arcane_engine.dll` — The compiled engine DLL.

## Notes

- You may need to adjust library paths if MinGW or libraries are not in standard locations.
- Ensure `libpng` is installed and accessible to the linker.
