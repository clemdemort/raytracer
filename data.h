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

//a voxel object is a basic box in which we raymarch a 3D texture
struct VoxelOBJ     //10 floats
{
    //Param 1
    float PosX,PosY,PosZ;
    //Param 2
    float SizeX,SizeY,SizeZ;
    //Param 3
    float RotX,RotY,RotZ;
    //Param 5
    float textureID;//could have been stored as an int but to keep things simple we will use floats
};

//here is a function to fill a heap array containing voxel data, it take a filler function as an argument and the array to be filled
//the other parameters are pretty self explanatory
void VoxelTex(uint8_t (*filler)(int,int,int),uint8_t *& data,int width,int height,int depth)
{
    data = new uint8_t[width*height*depth];
    for(int x = 0; x < width;x++)
        for(int y = 0; y < height;y++)
            for(int z = 0; z < depth;z++)
                data[uint32_t((x*height*depth)+(y*depth)+z)] = filler(x,y,z);
}

uint8_t voxSphere(int x,int y,int z)
{
    int center = 25;
    if(((x-center)*(x-center))+((y-center)*(y-center))+((z-center)*(z-center)))return 1;
    else return 0;
}



class scene
{
public:

    sphere * spherelist = new sphere[1];
    cube * cubelist = new cube[1];
    VoxelOBJ * voxellist = new VoxelOBJ[1];
    int numSpheres,numCubes,numVoxels;
    scene(int numofSpheres,int numofCubes, int numofVoxels)
    {
        delete[] spherelist;
        delete[] cubelist;
        delete[] voxellist;
        numSpheres = numofSpheres;
        numCubes = numofCubes;
        numVoxels = numofVoxels;
        spherelist = new sphere[numofSpheres];
        cubelist = new cube[numofCubes];
        voxellist = new VoxelOBJ[numofVoxels];
        //------------------------------------
        //this part will be responsible for generating the geometry so it should be easy enough to tweak

        //1st we will generate the spheres
        //to do so we need to set mins and max for positions and Size
        float maxPosX = 75;
        float minPosX = -75;
        float maxPosY = 25;
        float minPosY = 10;
        float maxPosZ = 75;
        float minPosZ = -75;

        float minSize = 1;
        float maxSize = 5;

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
            cubelist[i].PosX          = getrand(minPosX*100.0,maxPosX*100.0)/100;
            cubelist[i].PosY          = getrand(minPosY*100.0,maxPosY*100.0)/100;
            cubelist[i].PosZ          = getrand(minPosZ*100.0,maxPosZ*100.0)/100;
            cubelist[i].SizeX         = getrand(minSize*100.0,maxSize*100.0)/100;
            cubelist[i].SizeY         = getrand(minSize*100.0,maxSize*100.0)/100;
            cubelist[i].SizeZ         = getrand(minSize*100.0,maxSize*100.0)/100;
            cubelist[i].RotX          = getrand(0,3.14*100.0)/100;
            cubelist[i].RotY          = getrand(0,3.14*100.0)/100;
            cubelist[i].RotZ          = getrand(0,3.14*100.0)/100;
            cubelist[i].colourRED     = getrand(0,255)/255.0;
            cubelist[i].colourGREEN   = getrand(0,255)/255.0;
            cubelist[i].colourBLUE    = getrand(0,255)/255.0;
            cubelist[i].transparency  = getrand(0,255)/255.0;
            cubelist[i].roughthness   = getrand(0,255)/255.0;
        }
        for(int i = 0; i < numofVoxels; i++)
        {
            voxellist[i].PosX          = 100;
            voxellist[i].PosY          = 100;
            voxellist[i].PosZ          = 100;
            voxellist[i].SizeX         = getrand(minSize*100.0,maxSize*100.0)/100;
            voxellist[i].SizeY         = getrand(minSize*100.0,maxSize*100.0)/100;
            voxellist[i].SizeZ         = getrand(minSize*100.0,maxSize*100.0)/100;
            voxellist[i].RotX          = getrand(0,3.14*100.0)/100;
            voxellist[i].RotY          = getrand(0,3.14*100.0)/100;
            voxellist[i].RotZ          = getrand(0,3.14*100.0)/100;
            voxellist[i].textureID     = getrand(0,255)/255.0;
        }
    }
    void ToSSBOData(std::string param,float *& data)
    {
        if(param == "GET_SPHERE_DATA"){
            data = new float[numSpheres*9];
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
            data = new float[numCubes*14];
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
        if(param == "GET_VOXEL_DATA"){
            data = new float[numVoxels*10];
            for(int i = 0;i < numVoxels; i++)
            {
                data[(10*i)]    = voxellist[i].PosX;
                data[(10*i)+1]  = voxellist[i].PosY;
                data[(10*i)+2]  = voxellist[i].PosZ;
                data[(10*i)+3]  = voxellist[i].SizeX;
                data[(10*i)+4]  = voxellist[i].SizeY;
                data[(10*i)+5]  = voxellist[i].SizeZ;
                data[(10*i)+6]  = voxellist[i].RotX;
                data[(10*i)+7]  = voxellist[i].RotY;
                data[(10*i)+8]  = voxellist[i].RotZ;
                data[(10*i)+9]  = voxellist[i].textureID;

            }

        }
    }
//amogus
};

#endif
