# Orchid-2D-Engine (2014)

# General Information
This is a game engine playground I developped in 2014. It features:
- Music using ogg-files 
- 2D Tile-Based levels created in Tiled, imported using as xml
- simple tile-based renderer using DirectX 9.0c
- keyboard, mouse and gamepad support using Direct Input.

# Technologies Used
- [tinyxml2](https://github.com/leethomason/tinyxml2)
- [libogg](https://xiph.org/ogg/)
- [libvorbis](https://xiph.org/vorbis/)

# How To Run
1. Requirements:
[C++ Redistributable 2022 (v143)](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)

2. Compiled binary:
A compiled binary can be found in `snapshot`.
It requires the `data` to be in the same directory as the executable.

# How to Compile
1. Requirements:
- [Visual Studio Community](https://visualstudio.microsoft.com/de/downloads/)
  - Choose workloads: `Desktop development with C++` and `Game Development with C++`.
- [DirectX SDK 2010](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

2. Solution:
The solution found in the root directory is setup, so that you can compile the project and all of it's requirements.

# Illustrations

# Features

# Sources
- [Ludum Dare 29: Deep Blue Home by Jools64](https://web.archive.org/web/20170703054547/http://www.ludumdare.com/compo/ludum-dare-29/?action=preview&uid=17060): The test assets from this ludum dare entry were used.
- [Tiled](https://www.mapeditor.org/): Tiled Mapeditor was used to create the tilemap of the level.