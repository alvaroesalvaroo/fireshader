# Low-Level Bricks

ESTE DOCUMENTO ESTÁ OPTIMIZADO PARA VISUALIZARSE EN EL [REPOSITORIO DE GITLAB](https://gitlab.com/aruizgarcia14/pgpec3-a-game-in-c-with-opengl)

A high-performance, local multiplayer arcade game built with C++ and OpenGL. Based on the classic Breakout mechanics from the [Joey de Vries (LearnOpenGL) tutorial](https://learnopengl.com/In-Practice/2D-Game/Breakout), this version has been extended to support a 2-player mode.

![Game demo](./media/LowLevelBricksDemo.mp4)

## Code

Main classes in [src/](src/) folder are categorized as follows:

- **Rendering:** here we include _Shader_, _Texture_, _SpriteRenderer_, _TextRenderer_, and _PostProcessor_ classes.
- **GameObjects:** a basic _GameObject_ hierarchy, including _BallObject_, _Particle_ and _PowerUp_.
- **GameLogic:** here we include the interface _Scene_, implemented by _Game_ and _TwoPlayersGame_ classes. Also classes _GameLevel_ and _TwoPlayersLevel_ are included here.
- **AudioEngine:** was implemented using SDL_mixer. It is worth mentioning that we decided to include a quick cooldown (~50ms) so that the same sound is not reproduced too many times repeatedly. This avoids having an annoying sound when multiple collision are detected continuously (which happens quite often).

I finally decided to use inheritance in order to implement a 2-player variation of the game. This required refactoring the base _Game_ class into more granular methods to allow for overrides. I think it was a good practical decission to make the fewer possible variations, although Game class may have too many methods.

- Game Class: Acts as the base engine, handling the main loop, resource management, and collision logic.
- TwoPlayersGame Class: Inherits from Game. Makes variations to the methods of Game that references to Ball, Player, Lives or Particles, making it double. Also has a different logic to win the game.

## Dependencies
When making the build, three `.dll` files should be placed in the same directory as the executable: Freetype, SDL and SDL_mixer. For Windows OS, these can be found in:
- **Freetype**: [freetype/lib/win64(32)/freetype.dll](freetype/lib)
- **SDL2**: [SDL/SDL2-2.32.0/lib/x64(x86)/SDL2.dll](SDL/SDL2-2.32.0/lib)
- **SDL2_mixer**: [SDL/SDL2_mixer-2.8.1/lib/x64(x86)/SDL2_mixer.dll](SDL/SDL2_mixer-2.8.1/lib)

## Build 
Windows-x64 release build is in [build/Release-x64](build/Release-x64/) directory. I packaged a one-file build using [Enigma Virtual Box](https://enigmaprotector.com/en/aboutvb.html) software, which can be found in [build/](build/) directory.

## Assets
All assets included are from the amazing [LearnOpenGL by Joey de Vries](https://learnopengl.com) website, except for the music which was produced by me. It is worth mentioning that it uses samples from [Cab Calloway And His Orchestra - At The Clambake Carnival (1938)](https://www.youtube.com/watch?v=IaPFlQwf6hU&list=RDIaPFlQwf6hU&start_radio=1)