# raytracer

Here is my attempt at a small-ish real time ray-tracer using openGL 4.3. This is obviously not done so expect some progress in the comming weeks/months to this project.

Here are some pictures of what the raytracer currently does:
![image](https://user-images.githubusercontent.com/62178977/162789246-bd803e01-73f3-41ff-8927-6ea9204d446d.png)

![image](https://user-images.githubusercontent.com/62178977/166157106-da54191b-03c7-4767-a542-e68aa75f26a2.png)

![voxelBulb4](https://user-images.githubusercontent.com/62178977/166815062-0a550dd5-2660-4e24-848a-36e3bdeb5778.png)

![Screenshot_20220415_175045](https://user-images.githubusercontent.com/62178977/163593055-eb1cb7be-7e31-4edd-bc6a-b1112767eea6.png)


# Compiling on Linux:

the Compilation of this project is quite easy on linux:

First off, download the package for "glfw3" and any that might be missing (see compilation command): 

second off, install my configuration of glad which you can find here:
> https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on
download the zip file and put the header files in the "glad" folder, put "glad.c" alongside "main.cpp".

finaly in your terminal type in the project directory:
> g++ main.cpp glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o executable


# controls:

> W : Forward motion

> A : Left motion

> S : Backward motion

> D : Right motion

> N : Visualize normals

> TAB : Free Mouse

> EXIT: exit the application

PS: generating the voxel models might take a while so dont panic too much if the application takes a minute to load!

# To-Do list:

>~~add cubes~~ DONE

>~~add distance testing~~ DONE

>~~add shadow rays~~ DONE

>~~add shadow shading~~ DONE

>reflections,PBR,transparency

>set up acceleration structure

>~~add voxel objects~~ DONE

>make the voxel object creator more efficient and resourcefull(model loading ;D)

>octree implementation on the voxel objects or some form of RLE compression
