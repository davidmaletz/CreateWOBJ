# CreateWOBJ
Uses [assimp](https://github.com/assimp/assimp) to convert models from various formats to wobj files (for Aground Zero).

This was adapted from old code from a C++ engine I never finished developing (before even my first flash games were released). However, it's relatively straightforward, if you add an assimp include directory to the compiler and an assimp static or dynamic library to the linker, Main.cpp should compile on windows, and on other platforms with minor changes. You can also figure out the WOBJ file format from this code if you wish - it's a binary format that is straightforward to upload to the GPU to render static or node/bone animations.

Feel free to get the windows executable in the [releases](https://github.com/davidmaletz/CreateWOBJ/releases/tag/release), but I decided to make the source code available for everyone so it's easy to use and modify the WOBJ format - the only format that Aground Zero accepts.

# Usage

CreateWOBJ is a command line application that accepts an input file, output file and optional -writemeshes argument.

CreateWOBJ input output [-writemeshes]

CreateWOBJ supports bone and node animations, but not mesh animations (vertex-based animations, these are pretty rare nowadays). CreateWOBJ merges all meshes, materials and animations into one file - you’ll specify textures in xml. Aground Zero does not support multiple textures per wobj - either pack the textures into one mega-texture, or (if necessary) break the object into multiple wobj files.

While all meshes are merged, you can add -writemeshes as a third command line argument which will write the names and vertex subset for each mesh in the object - this is useful for making subsets.
