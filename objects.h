#ifndef OBJECTS_H
#define OBJECTS_H
 
#include "data.h"
//this function should be edited, it determines what should spawn in a given scene
void populatescene(scene &scene,GLuint voxATLAS){
	palette base;
	base.C[1] = uint32_t(0xff0000ff);
	base.C[2] = uint32_t(0x00ff00ff);
	base.C[3] = uint32_t(0x00ffffff);
	AppendPaletteList(scene,base);
	CreateVoxelOBJ(scene, voxATLAS,&voxBulb,
		{64, 64, 64},
		{0, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16,0);
	AppendVoxList(scene,
		{64, 64, 64},
		{0, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		4,0);
	CreateVoxelOBJ(scene, voxATLAS,&voxBulb,
		{128, 128, 128},
		{64, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16,0);
}

#endif
