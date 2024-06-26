<h1 align="center">
  Minimap, <a href="https://stackoverflow.com/questions/173199/can-someone-explain-per-pixel-collision-detection">Pixel Perfect Collision</a> & Timer/Stopwatch using <a href="https://github.com/libsdl-org/sdl12-compat">SDL 1.2</a> Library in C.
</h1>
<p align="center">
  <a href="https://github.com/libsdl-org/sdl12-compat"><img src="https://img.shields.io/badge/SDL-1.2-blue.svg?maxAge=2592000&amp;style=flat"></a>
</p>

<div align="center">

![screenshot](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMHhsY3BhdDRoaGlnYzk0M2xmem16MXR0MzZsa2gzczk3aDVhenpkMCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/TLwGlb55PEKJnMuD3h/giphy.gif)

![screenshot](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExbzEzMmdkbHJobGkyNXViNXV0MTVtamkxc2VlZzc1N2YxYzE4YmYwcSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/54quFw5JcDE347Sf94/giphy.gif)
</div>

## Description
Secondary functionalities along with a minimap for a 2D game made with C & SDL 1.2 Library :
* <strong>init_minimap</strong> : Initializes the minimap.
* <strong>MAJMinimap</strong> : Updates the minimap.
* <strong>afficher_minimap</strong> : Displays the minimap on the screen.
* <strong>liberer_minimap</strong> : Frees the resources allocated for the minimap.
* <strong>affichertempsen</strong> : Displays the elapsed time since the start time.
* <strong>affichertemps</strong> : Displays a mini stopwatch.
* <strong>GetPixel</strong> : Returns the color of the pixel at the given coordinates.
* <strong>collisionPP</strong> : Perfect Pixel Collision between the player and a specific color (black) in the background.
* <strong>sauvegarder</strong> : Saves the game state to a file.
* <strong>charger</strong> : Loads the game state from a file.

## How to use
* Call the desiered functions in your `main.c` with your own `resources`.
* For testing :<br>
  1- go inside the folder. <br>
  2- run makefile by typing `make`. <br>
  3- run the program by typing `./prog`. 

## Related
