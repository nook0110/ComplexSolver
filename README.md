# ComplexSolver
### Introduction

ComplexSolver uses LaTeX. It is optional to be installed. With LaTeX you can click on objects to see their equations.

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

### Building with CMake and MinGW

ComplexSolver uses SFML library, so you need it to be installed. There is a ```Find_SFML.cmake``` script (took from [here](https://github.com/eXpl0it3r/game-off-2016)) 
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
Chord AB ByTwoUnitPoints A B
```
Syntax is ```<Type> <Name> <Contruction> <Args...>```

Type:
* ```Point```
* ```UnitPoint```
* ```Line```
* ```Chord```

Name: make it up yourself! 
```But be careful with redefenition, also don`t forget about objects in presets```

### ```Point```
Construction:
* ```OnPlane```

Creates a point on a plane.

  Args: coords of point (x,y) (float)
 ```
 Point <Name> OnPlane 10 50
 ```
 This point isn`t pinned to plane, you can move it. Args are just to init point.
* ```IntersectionOfTwoLines```

Creates a point which is intersection of two lines (chords are lines too)

  Args: name of two lines (chords are lines too)
 ```
 Line AB ...
 Chord CD ...
 Point <Name> IntersectionOfTwoLines AB CD
 ```
* ```OnLine```

Creates a point which lies on line (chords are lines too)

  Args: name of line, name of two points (which are on this line) and coords of point (they will be casted to the nearest point on line)
```If you will write points that are not on the line, compiler will not say anything about that and program will have undefined behavior```
 ```
 Line AB ...
 Point C ... //Must be on the AB
 Point D ... //Must be on the AB
 Point <Name> OnLine AB C D 4 7
 ```
  This point isn`t pinned, you can move along the line it. Coords are just to init point.
* ```Projection```

Creates a point which is projection of point on a line (chords are lines too)

  Args: name of point and a line
 ```
 Point A ...
 Line BC ...
 Point <Name> Projection A BC
 ```

* ```byTwoPointsFixedRatio```

Creates a point which is a center of two point with masses

  Args: names of two points and their masses ```Masses must be integer numbers```
  
 ```
 Point A ...
 Point B ...
 Point <Name> byTwoPointsFixedRatio A B 2 3
 ```
  
  * ```Rotation90```

Rotates a point by 90 degrees

  Args: names of two points and sign (sign is -1 or 1)
  ```If sign isn`t 1 or -1, compiler will not say anything about that, programm will have undefined behavior```
 ```
 Point center ...
 Point preimage ...
 Point <Name> Rotation90 center preimage 1
 ```
 
   * ```Orthocenter```

Creates an orthocenter of three ```UnitPoint```s

  Args: names of three ```UnitPoint```s
 ```
 UnitPoint A ...
 UnitPoint B ...
 UnitPoint C ...
 Point <Name> Orthocenter A B C
 ```
 
  * ```Barycenter```

Creates an barycenter of three points

  Args: names of three points
 ```
 Point A ...
 Point B ...
 Point C ...
 Point <Name> Barycenter A B C
 ```
 
### ```UnitPoint```

* ```OnCircle```

Creates a point on circle

  Args: (float) coordinates (x,y) ```Coord will be normilized (x^2 + y^2 = 1)```
  
 ```
 UnitPoint <Name> OnCircle 3 2
 ```
 
 This point isn't pinned, you can mive it along the circle
 
 * ```CentralProjection```

Creates a point on circle which is central projection of first through second (it must be ```UnitPoint```)

  Args: name of point and ```UnitPoint```
  
 ```
 Point A ...
 UnitPoint B ...
 UnitPoint <Name> CentralProjection A B
 ```
 
 * ```IntersectionPerpendicularChord```

Creates a point wihch is intersection of ```UnitCircle``` and chord which goes throug ```UnitPoint``` and perpendicular to another chord

  Args: name of chord and name of ```UnitPoint```
  
 ```
 Chord AB ...
 UnitPoint C ...
 UnitPoint <Name> IntersectionPerpendicularChord AB C
 ```
 
  * ```IntersectionParallelChord```

Creates a point wihch is intersection of ```UnitCircle``` and chord which goes throug ```UnitPoint``` and parallel to another chord

  Args: name of chord and name of ```UnitPoint```
  
 ```
 Chord AB ...
 UnitPoint C ...
 UnitPoint <Name> IntersectionParallelChord AB C
 ```
 
 ### ```Line```

* ```ByTwoPoints```

Creates a line through two points

  Args: names of two points
  
 ```
 Point A ...
 Point B ...
 Line <Name> ByTwoPoints A B
 ```
 
 * ```Tangent```

Creates a tangent to ```UnitCircle``` though a ```UnitPoint```

  Args: names ```UnitPoint```
  
 ```
 UnitPoint A ...
 Line <Name> Tangent A
 ```
 
 * ```Parallel```

Creates a parallel line to another line through another point

  Args: names of line and point
  
 ```
 Line AB ...
 Point C ...
 Line <Name> Parallel AB C
 ```
 
 * ```Perpendicular```

Creates a perpendicular line to another line through another point

  Args: names of point and line
  
 ```
 Point A ...
 Line BC ...
 Line <Name> Perpendicular A BC
 ```
 
  ### ```Chord```

* ```ByTwoUnitPoints```

Creates a chord through two ```UnitPoint```s

  Args: names of two ```UnitPoint```s
  
 ```
 UnitPoint A ...
 UnitPoint B ...
 Chord <Name> ByTwoUnitPoints A B
 ```
 
## Have a nice experience!
