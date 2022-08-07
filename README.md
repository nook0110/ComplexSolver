# ComplexSolver
### Building with CMake and MinGW

SFML_Project uses SFML library, so you need it to be installed. There is a ```Find_SFML.cmake``` script (took from [here](https://github.com/eXpl0it3r/game-off-2016)) 
that may find installed SFML (for example, if you use MSYS2 to install libraries), but if it didn't, try to add ```-DSFML_ROOT=C:/..``` to CMake configure command 
(check [this](https://stackoverflow.com/questions/45671299/linking-sfml-with-cmake-windows-mingw)).

Now, if you have MinGW and CMake:

```
cd .../ComplexSolver
cmake -G "MinGW Makefiles" -S "" -B "build"
```

If everything went fine, CMake will create a build directory and write its files here.

```
cmake --build build
```

You have built ComplexSolver! Now just

```
cd build
sfml.exe
```
___

### Building with Visual Studio

Open project with Visual Studio

Edit paths to SFML in ```Project->Properties->C/C++->General``` for headers and ```Project->Properties->Linker->General``` for libraries
(check [this](https://www.sfml-dev.org/tutorials/2.5/start-vc.php) for more info)

If everything went fine, you can compile a project.

After compilation and running you would probably see a message: 
```
sfml-graphics-2.dll was not found
```

You need to put bin files (These files can be found in ```<sfml-install-path/bin>```) near ```.exe```
___

# Using the program

* Start ```.exe```
* Set the settings

  Max framerate:
  * ```60``` - Fps is limited to 60
  * ```Vertical Sync``` - Vertical Synchronization
  * ```Unlimited``` - Fps has no limits

  Presets:
  * ```Empty``` - Nothing is presetted (only O-point and unit circle)
  * ```Triangle``` - Creates Triangle ```ABC``` inscribed into unit circle
  * ```Incenter``` - Creates Triangle ```ABC``` inscribed into unit circle,incenter ```I```, and centers of arcs: ```Da```, ```Db```, ```Dc```
  * ```Orthocenter``` - Creates Triangle ```ABC``` inscribed into unit circle and orthocenter ```H```

* Press ```Enter```
* Build the problem with instruments at the top of the screen

![image](https://user-images.githubusercontent.com/77803419/183294735-ea2387ea-ad7c-4346-bb99-cba8fe3504e2.png)

* To prove the theorem use this button:

![SwitchConstruction](https://user-images.githubusercontent.com/77803419/183294703-56ab02ab-7287-4126-8716-179e3651d0da.png)

* Choose what you need to prove

![image](https://user-images.githubusercontent.com/77803419/183294857-6f444bde-d722-4a56-93f8-3f89dda62f99.png)


* Wait until the box on the bottom right gives you the result

![image](https://user-images.githubusercontent.com/77803419/183294843-7d186c4f-6ea3-449f-9e6d-b46f947432ef.png)


# Compiling files

You can compile files such as ```Pascal's theorem.txt```

Just drag them onto the ```.exe```

# How to make this files

Saving picture isn`t in project now.
But if you want, you can write this file yourself.

### Settings
In first line you need to write settings
```
Settings 60 Empty
```

Syntax is: ```Settings <FPS> <Presets>```

FPS:
* ```Unlimited``` - Fps has no limits
* ```VS``` - Vertical Synchronization
* ```60``` - Fps is limited to 60

Presets:
* ```Empty``` - Nothing is presetted (only O-point and unit circle)
* ```Triangle``` - Creates Triangle ```ABC``` inscribed into unit circle
* ```Orthocenter``` - Creates Triangle ```ABC``` inscribed into unit circle and orthocenter ```H```
* ```Incenter``` - Creates Triangle ```ABC``` inscribed into unit circle, incenter ```I```, and centers of arcs: ```Da```, ```Db```, ```Dc```

## Objects
```
Chord X1 ByTwoUnitPoints A B
```
Syntax is ```<Type> <Name> <Contruction> <Args...>```

Type:
* ```Point```
* ```UnitPoint```
* ```Line```
* ```Chord```

Name: make it up yourself! 
```But be careful with redefenition, also don`t forget about objects in presets```

### Point
Construction:
* ```OnPlane```

Creates a point on a plane.

  Args: Coords of point (x,y)
 ```
 Point point_name OnPlane 10 50
 ```
 This point isn`t pinned to plane, you can move it. Args are just to init point.
* ```IntersectionOfTwoLines```

Creates a point which is intersection of two lines (chords are lines too)

  Args: name of two lines (chords are lines too)
 ```
 Line line_1 ...
 Chord chord_1 ...
 Point point_name IntersectionOfTwoLines line_1 chord_1
 ```
* ```OnLine```

Creates a point which lies on line (chords are lines too)

  Args: name of line, name of two points (which are on this line) and coords of point (they will be casted to the nearest point on line)
```If you will write points that are not on the line, compiler will not say anything about that and programm will have undefined behaviour```
 ```
 Line line_1 ...
 Point point_1 ... //Must be on the line_1
 Point point_2 ... //Must be on the line_1
 Point point_name OnLine point_1 point_2
 ```
  This point isn`t pinned, you can move along the line it . Coords are just to init point.
* ```Projection```

Creates a point which is projection of point on a line (chords are lines too)

  Args: point and a line
 ```
 Point point_1 ...
 Line line_1 ...
 Point point_name Projection point_1 line_1
 ```
  point_name is projection of point_1 on line_1
* ```byTwoPointsFixedRatio```

Creates a point which coordin

  Args: point and a line
 ```
 Point point_1 ...
 Line line_1 ...
 Point point_name Projection point_1 line_1
 ```
  Point_name is projection of point_1 on line_1 
  
...

## Have a nice experience!
