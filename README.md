# raytracer

Here is my attempt at a small-ish real time ray-tracer using openGL 4.3. This is obviously not done so expect some progress in the comming weeks/months to this project.

Here is a picture of what the raytacer currently does:

![raytracergithub](https://user-images.githubusercontent.com/62178977/154512844-2d3fbf63-fe5f-4a1f-9de2-2b30d1e03309.png)

# Compiling on Linux:

the Compilation of this project is quite easy on linux:

First off, download the package for "glfw3" and any that might be missing (see compilation command): 

second off, install my configuration of glad which you can find here:
> https://glad.dav1d.de/generated/tmp7s7wppfaglad/

finaly in you terminal type:
> g++ main.cpp glad/glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o executable


# controls:

> W : Forward motion

> A : Left motion

> S : Backward motion

> D : Right motion

> TAB : Free Mouse

> EXIT: exit the application
