#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <time.h>
#include <math.h>

float getrand(int min ,int max)
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
    cube * cubelist = new cube[1];
    int numSpheres,numCubes;
    scene(int numofSpheres,int numofCubes)
    {
        delete[] spherelist;
        delete[] cubelist;
        numSpheres = numofSpheres;
        numCubes = numofCubes;
        spherelist = new sphere[numofSpheres];
        cubelist = new cube[numofCubes];
        //------------------------------------
        //this part will be responsible for generating the geometry so it should be easy enough to tweak

        //1st we will generate the spheres
        //to do so we need to set mins and max for positions and Size
        float maxPosX = 500;
        float minPosX = -500;
        float maxPosY = 50;
        float minPosY = 0;
        float maxPosZ = 500;
        float minPosZ = -500;

        float minSize = 1;
        float maxSize = 20;

        //initialising the random seed
        srand(time(NULL));

        for(int i = 0; i < numofSpheres; i++)
        {
            spherelist[i].PosX = getrand(minPosX*100.0,maxPosX*100.0)/100;        //we want a bit more floating point precision
            spherelist[i].PosY = getrand(minPosY*100.0,maxPosY*100.0)/100;
            spherelist[i].PosZ = getrand(minPosZ*100.0,maxPosZ*100.0)/100;

            spherelist[i].Size = getrand(minSize*100,maxSize*100)/100;

            spherelist[i].colourRED     = getrand(0,255)/255.0;
            spherelist[i].colourGREEN   = getrand(0,255)/255.0;
            spherelist[i].colourBLUE    = getrand(0,255)/255.0;
            spherelist[i].transparency  = getrand(0,255)/255.0;     //these last two wont do much as of yet but they will play a key role in the future
            spherelist[i].roughthness   = getrand(0,255)/255.0;
        }
        for(int i = 0; i < numofCubes; i++)
        {
            cubelist[i].PosX = getrand(minPosX*100.0,maxPosX*100.0)/100;        //we want a bit more floating point precision
            cubelist[i].PosY = getrand(minPosY*100.0,maxPosY*100.0)/100;
            cubelist[i].PosZ = getrand(minPosZ*100.0,maxPosZ*100.0)/100;
            cubelist[i].SizeX = getrand(minSize*100.0,maxSize*100.0)/100;
            cubelist[i].SizeY = getrand(minSize*100.0,maxSize*100.0)/100;
            cubelist[i].SizeZ = getrand(minSize*100.0,maxSize*100.0)/100;
            cubelist[i].RotX = 0;
            cubelist[i].RotY = 0;
            cubelist[i].RotZ = 0;
            cubelist[i].colourRED     = getrand(0,255)/255.0;
            cubelist[i].colourGREEN   = getrand(0,255)/255.0;
            cubelist[i].colourBLUE    = getrand(0,255)/255.0;
            cubelist[i].transparency  = getrand(0,255)/255.0;
            cubelist[i].roughthness   = getrand(0,255)/255.0;
        }
    }
    void ToSSBOData(std::string param,float *& data)
    {
        if(param == "GET_SPHERE_DATA"){
            for(int i = 0;i < numSpheres; i++)
            {
                data[(9*i)]   = spherelist[i].PosX;
                data[(9*i)+1] = spherelist[i].PosY;
                data[(9*i)+2] = spherelist[i].PosZ;
                data[(9*i)+3] = spherelist[i].Size;
                data[(9*i)+4] = spherelist[i].colourRED;
                data[(9*i)+5] = spherelist[i].colourGREEN;
                data[(9*i)+6] = spherelist[i].colourBLUE;
                data[(9*i)+7] = spherelist[i].transparency;
                data[(9*i)+8] = spherelist[i].roughthness;
            }

        }
        if(param == "GET_CUBE_DATA"){
            for(int i = 0;i < numCubes; i++)
            {
                data[(14*i)]    = cubelist[i].PosX;
                data[(14*i)+1]  = cubelist[i].PosY;
                data[(14*i)+2]  = cubelist[i].PosZ;
                data[(14*i)+3]  = cubelist[i].SizeX;
                data[(14*i)+4]  = cubelist[i].SizeY;
                data[(14*i)+5]  = cubelist[i].SizeZ;
                data[(14*i)+6]  = cubelist[i].RotX;
                data[(14*i)+7]  = cubelist[i].RotY;
                data[(14*i)+8]  = cubelist[i].RotZ;
                data[(14*i)+9]  = cubelist[i].colourRED;
                data[(14*i)+10] = cubelist[i].colourGREEN;
                data[(14*i)+11] = cubelist[i].colourBLUE;
                data[(14*i)+12] = cubelist[i].transparency;
                data[(14*i)+13] = cubelist[i].roughthness;
            }

        }
    }
//amogus
};

#endif
