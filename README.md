# Conway's Game of Life - A game written using C++ and SDL2

The project implements a [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) with using C++, SDL2 and CMake
It supports settings via command line parameters and it has interactive control

## 🔥 Features 

- **Flexible settings**: 
    You can change the sizes of the game field and window
- **Interactivity**: 
    - Add or remove cells by a mouse click
    - Control speed of the game 
    - Pause or play
    - Clear the field
    - Set up the sizes

## 📦 Setting Up

### Requirements
- C++20 Compiler
- CMake 3.18+
- SDL2 Library

### Building
```bash
git clone https://github.com/seweex/game-of-life.git
cd game-of-life
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## 🎮 Control
- **Q or E**: Speed up or slow down the game
- **C**: Clear the game field
- **Space**: Pause and continue
- **LMB**: Add or remove a cell
- **ESC**: Exit

## ⚙️ Launch parameters

### Parameters
- **-fieldwidth**: Set up the width of the field, in range [1, *Your Screen Width*], default: 25
- **-fieldheight**: Set up the height of the field, in range [1, *Your Screen Height*], default: 25
- **-windowwidth**: Set up the width of the window, in range [*-fieldwidth Value*, *Your Screen Width*], default: 800
- **-windowheight**: Set up the height of the window, in range [*-fieldheight Value*, *Your Screen Height*], default: 800
- **-tickrate**: Set up the game speed, in range [1, 128], default: 3

### Example
```bash
./game-of-life -fieldwidth=50 -fieldheight=50 -tickrate=10
```

## ⭐ Feedback

I'll be glad if you help me improve the code :D
