# raytracer

Here is my attempt at a small-ish real time ray-tracer using openGL 4.3. This is obviously not done so expect some progress in the comming weeks/months to this project.

Here are some pictures of what the raytracer currently does:
![image](https://user-images.githubusercontent.com/62178977/162789246-bd803e01-73f3-41ff-8927-6ea9204d446d.png)

![image](https://user-images.githubusercontent.com/62178977/166157106-da54191b-03c7-4767-a542-e68aa75f26a2.png)

![voxelBulb4](https://user-images.githubusercontent.com/62178977/166815062-0a550dd5-2660-4e24-848a-36e3bdeb5778.png)

![Screenshot_20220415_175045](https://user-images.githubusercontent.com/62178977/163593055-eb1cb7be-7e31-4edd-bc6a-b1112767eea6.png)


# Compiling on Linux:

the Compilation of this project is quite easy on linux:

download the GLFW3 package from your distributions package repo,
on arch for instance it's "glfw3" (if it asks you choose X11 but wayland should work too)

finaly in your terminal type in the project directory:
> make

It will run automatically

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

>make the voxel object creator more efficient and resourcefull(model loading ;D) ONGOING PROGRESS

>octree implementation on the voxel objects or some form of RLE compression
