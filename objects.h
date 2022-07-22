#ifndef OBJECTS_H
#define OBJECTS_H
 
#include "data.h"
//this function should be edited, it determines what should spawn in a given scene
void populatescene(scene &scene,GLuint voxATLAS){
	palette base;
	base.C[1] = 0xff000000;
	base.C[2] = 0x00ff0000;
	base.C[3] = 0x00ffff00;
	AppendPaletteList(scene,base);
	CreateVoxelOBJ(scene, voxATLAS,&voxBulb,
		{100, 100, 100},
		{0, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16,0);
	AppendVoxList(scene,
		{100, 100, 100},
		{0, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		4,0);
}

#endif
