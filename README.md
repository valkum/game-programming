# Himmel
Game Programming WS15/16  
Rudi Floren 999999  
Sebastian Bockholt 322941  
Kim Brose 322935  
[![Build Status](https://magnum.travis-ci.com/valkum/game-programming.svg?token=fG6GG7VSsLF7HUxqCHii&branch=master)](https://magnum.travis-ci.com/valkum/game-programming)

## Screenshot
![Demo Screenshot](assets/images/screenshot.png)

## Instructions
You are a generic superhero flying through a city skyline!

Use A and D keys to move left or right respectively and evade buildings in your way.  
Use Esc to quit.

### Debug keys:  
- **P** to switch to wireframe debug view
- **F** to show camera position

## Dependencies
- [GLFW](http://www.glfw.org/)
- GLM
- OpenGL
- ACGL
- json (included)
- nanovg (included)

## Build
For automated build use CMake with pkg-config.

Also includes a bash script ./build.sh for ease of use.  
The Executable will be ./bin/Game