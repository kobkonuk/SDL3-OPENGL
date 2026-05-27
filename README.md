I gave up on this because I think I need better fundamentals before doing any big project whatsoever.


For everybody else, just use what I made as a framework incase you want to incorporate OpenGL into SDL3.


thecherno has amazing tutorials on OpenGL, however he uses GLFW in the one I watched. I havent fully implemented MVP yet, maybe thecherno has a tutorial on that


out of respect for nothings/stb please dont use AI or consider not using STB entirely


if you want to use stb like me just download stb_image from nothings/stb and put it inside your project and include it

just do it the normal way. or:

(idk how to do it on windows)

download stb_image from nothings/stb
```bash
sudo mkdir /usr/local/bin/STB/
sudo mkdir /usr/local/src/STB/
cp stb_image.h /usr/local/bin/STB/
cp stb_image.h /usr/local/src/STB/
cd /usr/local/src/STB/
touch stb_image_impl.c
```
inside stb_image_impl.c
```c
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
```
back in /usr/local/bin/STB
```bash
gcc -02 -c stb_image_impl.c -o stb_image.o
ar rcs /usr/local/lib/libSTB.a stb_image.o
```
i think, idk actually. try it and see if it works im too lazy to double check. Maybe this is a package in the AR or AUR and my dumbass is manually doing it

i never bothered to check

```c
#include <STB/stb_image.h>
```
done
