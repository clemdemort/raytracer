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

struct ivec3
{
    int x,y,z;
};

struct vec3
{
    float x,y,z;
};
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
struct VoxelOBJ     //15 floats
{
    //Param 1
    float PosX,PosY,PosZ;
    //Param 2
    float SizeX,SizeY,SizeZ;
    //Param 3
    float RotX,RotY,RotZ;
    //Param 5
    float texOffsetX,texOffsetY,texOffsetZ;     //where in the atlas will it look for?(point of begining)
    float SampleSizeX,SampleSizeY,SampleSizeZ;  //how much will it look for?          (size of sample)
};

//here is an example function that generates a sphere
uint8_t voxSphere(ivec3 P,ivec3 S)
{
    float cx = float(2.*P.x/S.x)-1;
    float cy = float(2.*P.y/S.y)-1;
    float cz = float(2.*P.z/S.z)-1;
	/*int coord1 = (P.x == 1 && P.y == 0 && P.z == 0);
    int coord2 = (P.x == 0 && P.y == 0 && P.z == 35);
	if(coord1 || coord2) return 1;
    return 0;*/
    if((cx*cx)+(cy*cy)+(cz*cz)<1)return 1;
    else return 0;
}
uint8_t voxBulb(ivec3 P, ivec3 S)
{
    float sample = 8;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb1(ivec3 P, ivec3 S)
{
    float sample = 9;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}


uint8_t voxBulb2(ivec3 P, ivec3 S)
{
    float sample = 10;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb3(ivec3 P, ivec3 S)
{
    float sample = 11;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb4(ivec3 P, ivec3 S)
{
    float sample = 12;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb5(ivec3 P, ivec3 S)
{
    float sample = 13;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb6(ivec3 P, ivec3 S)
{
    float sample = 14;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb7(ivec3 P, ivec3 S)
{
    float sample = 15;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}
uint8_t voxBulb8(ivec3 P, ivec3 S)
{
    float sample = 16;
    int iterCount = 8;
    int i = 0;
    float cx = 1.1*(P.x-S.x/2.f)/float(S.x/2.0f);//this is done so the whole bulb can be seen
    float cy = 1.1*(P.y-S.y/2.f)/float(S.y/2.0f);
    float cz = 1.1*(P.z-S.z/2.f)/float(S.z/2.0f);
    float r = 0;
    float theta;
    float phi;
    float wx = cx,wy = cy,wz = cz;
    while(i < iterCount && r <= 2)
    {
        r = sqrt(wx*wx + wy*wy + wz*wz);
        theta = atan2(sqrt(wx*wx+wy*wy),wz);
        phi = atan2(wy,wx);
        float X2 = pow(r,sample)*sin(theta*sample)*cos(phi*sample);
        float Y2 = pow(r,sample)*sin(theta*sample)*sin(phi*sample);
        float Z2 = pow(r,sample)*cos(theta*sample);
        wx = X2 + cx;
        wy = Y2 + cy;
        wz = Z2 + cz;
        i++;
    }
     return (i >= iterCount);
}










class scene
{
public:

    sphere * spherelist = new sphere[1];
    cube * cubelist = new cube[1];
    VoxelOBJ * voxellist = new VoxelOBJ[1];
    int numSpheres,numCubes,numVoxels;
    scene(int numofSpheres,int numofCubes)
    {
        delete[] spherelist;
        delete[] cubelist;
        delete[] voxellist;
        numSpheres = numofSpheres;
        numCubes = numofCubes;
        spherelist = new sphere[numofSpheres];
        cubelist = new cube[numofCubes];
        voxellist = new VoxelOBJ[1];
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
            data = new float[numVoxels*15];
            for(int i = 0;i < numVoxels; i++)
            {
                data[(15*i)]    = voxellist[i].PosX;
                data[(15*i)+1]  = voxellist[i].PosY;
                data[(15*i)+2]  = voxellist[i].PosZ;
                data[(15*i)+3]  = voxellist[i].SizeX;
                data[(15*i)+4]  = voxellist[i].SizeY;
                data[(15*i)+5]  = voxellist[i].SizeZ;
                data[(15*i)+6]  = voxellist[i].RotX;
                data[(15*i)+7]  = voxellist[i].RotY;
                data[(15*i)+8]  = voxellist[i].RotZ;
                data[(15*i)+9]  = voxellist[i].texOffsetX;
                data[(15*i)+10] = voxellist[i].texOffsetY;
                data[(15*i)+11] = voxellist[i].texOffsetZ;
                data[(15*i)+12] = voxellist[i].SampleSizeX;
                data[(15*i)+13] = voxellist[i].SampleSizeY;
                data[(15*i)+14] = voxellist[i].SampleSizeZ;
            }

        }
    }
//amogus
};


void AppendVoxList(scene & world,ivec3 TexSize, ivec3 TexOffset,vec3 Pos,vec3 Rot,float size)
{
    //creating the readable voxel object for the class
    VoxelOBJ obj;
    obj.PosX = Pos.x;
    obj.PosY = Pos.y;
    obj.PosZ = Pos.z;
    obj.RotX = Rot.x;
    obj.RotY = Rot.y;
    obj.RotZ = Rot.z;
    obj.texOffsetX = TexOffset.x;
    obj.texOffsetY = TexOffset.y;
    obj.texOffsetZ = TexOffset.z;
    obj.SampleSizeX = TexSize.x;
    obj.SampleSizeY = TexSize.y;
    obj.SampleSizeZ = TexSize.z;
    //calculating the relative size
    float sizeX,sizeY,sizeZ;
    sizeX = size;  sizeY = size;   sizeZ = size;
    if(TexSize.x >= TexSize.y && TexSize.x >= TexSize.z)//x is the biggest
        {sizeX = size;      sizeY = size * (float(TexSize.y)/float(TexSize.x));     sizeZ = size * (float(TexSize.z)/float(TexSize.x));}
    if(TexSize.y >= TexSize.x && TexSize.y >= TexSize.z)//y is the biggest
        {sizeY = size;      sizeX = size * (float(TexSize.x)/float(TexSize.y));     sizeZ = size * (float(TexSize.z)/float(TexSize.y));}
    if(TexSize.z >= TexSize.x && TexSize.z >= TexSize.y)//z is the biggest
        {sizeZ = size;      sizeY = size * (float(TexSize.y)/float(TexSize.z));     sizeX = size * (float(TexSize.x)/float(TexSize.z));}

    obj.SizeX = sizeX;
    obj.SizeY = sizeY;
    obj.SizeZ = sizeZ;
    //sending the data to the class
    world.numVoxels+=1;
    VoxelOBJ * temp = world.voxellist;
    world.voxellist = new VoxelOBJ[world.numVoxels];
    for(int i = 0;i < world.numVoxels-1;i++)
    {
        world.voxellist[i] = temp[i];
    }
    world.voxellist[world.numVoxels-1] = obj;
    free(temp);
}
void FillvoxelTex(GLuint texture,uint8_t(*filler)(ivec3,ivec3),ivec3 TexSize, ivec3 TexOffset)
{
    uint8_t * data = new uint8_t[TexSize.x*TexSize.y*TexSize.z];
    for(int x = 0; x < TexSize.x;x++){
        for(int y = 0; y < TexSize.y;y++){
            for(int z = 0; z < TexSize.z;z++){
                data[uint64_t((x*TexSize.y*TexSize.z)+(y*TexSize.z)+z)] = filler({x,y,z},TexSize);
            }        }
    }
	int a = TexSize.x;
	int b = TexSize.y;
	int c = TexSize.z;
	//sending data into the texture we specified:
    glBindTexture(GL_TEXTURE_3D, texture);
    glTexSubImage3D(GL_TEXTURE_3D,
        0,                // Mipmap number
        TexOffset.x, TexOffset.y, TexOffset.z,          // xoffset, yoffset, zoffset
        c, b, a, // width, height, depth
        GL_RED_INTEGER,         // format
        GL_UNSIGNED_BYTE, // type
        data);
    free(data);//freeing space once we've used it
}
void CreateVoxelOBJ(scene & world,GLuint texture,uint8_t(*filler)(ivec3,ivec3),ivec3 TexSize, ivec3 TexOffset,vec3 Pos,vec3 Rot,float size)
{
    //first we fill the 3D texture
    //----------------------------
    FillvoxelTex(texture,filler,TexSize,TexOffset);
    //second we code the information so that our class "scene" knows what to make of it
    //---------------------------------------------------------------------------------
    AppendVoxList(world,TexSize,TexOffset,Pos,Rot,size);
    //all done! :D
}


#endif
