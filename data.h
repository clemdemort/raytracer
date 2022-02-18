#ifndef DATA_H
#define DATA_H

struct geometry
{   //15 variables so 15*32 bits = 480 Bits = 60 Bytes
    int Type;                   //planes = 0, spheres = 1, cubes = 2
    //Param 1
    float PosX,PosY,PosZ;
    //Param 2
    float SizeX,SizeY,SizeZ;    //planes will have all three set to 0
    //Param 3
    float RotX,RotY,RotZ;       //planes will use rotations as normalized normals
    //Param 4
    float colourRED,colourGREEN,colourBLUE,transparency,roughthness;
};



#endif
