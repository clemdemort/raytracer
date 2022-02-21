#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <GLFW/glfw3.h>

int getrand(int min ,int max)
{
    return min+(rand() % (max-min));
}


struct sphere   //9 floats
{
    //Param 1
    float PosX,PosY,PosZ;
    //Param 2
    float Size;
    //Param 3
    float colourRED,colourGREEN,colourBLUE,transparency,roughthness;
};
struct plane    //11 floats
{
    //Param 1
    float PosX,PosY,PosZ;
    //Param 2
    float normX,normY,normZ;
    //Param 3
    float colourRED,colourGREEN,colourBLUE,transparency,roughthness;
};
struct cube     //14 floats
{
    //Param 1
    float PosX,PosY,PosZ;
    //Param 2
    float SizeX,SizeY,SizeZ;
    //Param 3
    float RotX,RotY,RotZ;
    //Param 4
    float colourRED,colourGREEN,colourBLUE,transparency,roughthness;
};
class scene
{
public:
    sphere * spherelist = new sphere[1];
    plane * planelist = new plane[1];
    cube * cubelist = new cube[1];
    int numSpheres,numPlanes,numCubes;
    scene(int numofSpheres,int numofPlanes,int numofCubes)
    {
        delete[] spherelist;
        delete[] planelist;
        delete[] cubelist;
        numSpheres = numofSpheres;
        numPlanes = numofPlanes;
        numCubes = numofCubes;
        spherelist = new sphere[numofSpheres];
        planelist = new plane[numofPlanes];
        cubelist = new cube[numofCubes];
        //------------------------------------
        //this part will be responsible for generating the geometry so it should be easy enough to tweak

        //1st we will generate the plane on which the scene resides
        //for now only one since i want to keep things simple but eventually we wil have more
        planelist[0].PosX = 0;
        planelist[0].PosY = 1;
        planelist[0].PosZ = 0;

        planelist[0].normX = 0;
        planelist[0].normY = 2;     //the plane will face up
        planelist[0].normZ = 0;

        planelist[0].colourRED = 0.1;
        planelist[0].colourGREEN = 0.7;
        planelist[0].colourBLUE = 0.1;
        planelist[0].transparency = 0;
        planelist[0].roughthness = 1;

        //2nd we will generate the spheres
        //to do so we need to set mins and max for positions and Size
        float maxPosX = 50;
        float minPosX = -50;
        float maxPosY = 50;
        float minPosY = 0;
        float maxPosZ = 50;
        float minPosZ = -50;

        float minSize = 0.01;
        float maxSize = 5;

        //initialising the random seed
        srand(glfwGetTime());

        for(int i = 0; i < numofSpheres; i++)
        {
            spherelist[i].PosX = getrand(minPosX*100,maxPosX*100)/100.0;        //we want a bit more floating point precision
            spherelist[i].PosY = getrand(minPosY*100,maxPosY*100)/100.0;
            spherelist[i].PosZ = getrand(minPosZ*100,maxPosZ*100)/100.0;

            spherelist[i].Size = getrand(minSize*100,maxSize*100)/100.0;

            spherelist[i].colourRED     = getrand(0,255)/255.0;
            spherelist[i].colourGREEN   = getrand(0,255)/255.0;
            spherelist[i].colourBLUE    = getrand(0,255)/255.0;
            spherelist[i].transparency  = getrand(0,255)/255.0;     //these last two wont do much as of yet but they will play a key role in the future
            spherelist[i].roughthness   = getrand(0,255)/255.0;
        }
    }
//amogus
};

#endif
