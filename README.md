# raytraycer

here is the code for a small-ish real time ray-tracer using openGL 4.3
I am now using Arch based linux so compiling is much easier:

> g++ main.cpp glad/glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o executable

also make sure to install my configuration of glad:
> https://glad.dav1d.de/generated/tmp7s7wppfaglad/

Here is a picture of what the raytacer currently does:

![raytracergithub](https://user-images.githubusercontent.com/62178977/154512844-2d3fbf63-fe5f-4a1f-9de2-2b30d1e03309.png)


#controls:
> W : Forward motion
> A : Left motion
> S : Backward motion
> D : Right motion
> TAB : Free Mouse
> EXIT: exit the application
