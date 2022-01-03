# SFML_project
### Building with CMake and MinGW

SFML_Project uses SFML library, so you need it to be installed. There is a ```Find_SFML.cmake``` script (took from [here](https://github.com/eXpl0it3r/game-off-2016)) 
that may find installed SFML (for example, if you use MSYS2 to install libraries), but if it didn't, try to add ```-DSFML_ROOT=C:/..``` to CMake configure command 
(check [this](https://stackoverflow.com/questions/45671299/linking-sfml-with-cmake-windows-mingw)).

Now, if you have MinGW and CMake:

```
cd .../SFML_Project
cmake -G "MinGW Makefiles" -S "" -B "build"
```

If everything went fine, CMake will create a build directory and write its files here.

```
cmake --build build
```

You have built SFML_Project! Now just

```
cd build
sfml.exe
```
___

### Building with Visual Studio

???
___




Have a nice experience!