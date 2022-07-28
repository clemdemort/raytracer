#ifndef OBJECTS_H
#define OBJECTS_H
 
#include "data.h"
//this function should be edited, it determines what should spawn in a given scene
void populatescene(scene &scene,GLuint voxATLAS){
	srand(time(NULL));
        	
	palette base;
	base.C[1] = uint32_t(0xff0000ff);
	base.C[2] = uint32_t(0x00ff00ff);
	base.C[3] = uint32_t(0x00ffffff);
	base.C[4] = uint32_t(0xf4d714ff);
	base.C[79] = uint32_t(0xff00aaff);
	AppendPaletteList(scene,base);
	//printf("test");
	palette base2;
	base2.C[1] = uint32_t(0x000000ff);
	base2.C[2] = uint32_t(0x202020ff);
	base2.C[3] = uint32_t(0xffffffff);
	base2.C[4] = uint32_t(0x00aaffff);
	base2.C[5] = uint32_t(0xff00aaff);
	base2.C[79] = uint32_t(0xffffffff);
	AppendPaletteList(scene,base2);
		
	ImportVoxelOBJ(scene, voxATLAS,"models/doom.vox",
		{0, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16);
	ImportVoxelOBJ(scene, voxATLAS,"models/chr_knight.vox",
		{116, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16);
	ImportVoxelOBJ(scene, voxATLAS,"models/dragon.vox",
		{200, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16);
	ImportVoxelOBJ(scene, voxATLAS,"models/monu2.vox",
		{400, 0, 0},
		{getrand(-75 * 100.0, 75 * 100.0) / 100, getrand(0 * 100.0, 75 * 100.0) / 100, getrand(-75 * 100.0, 75 * 100.0) / 100},
		{0 * getrand(-75 * 100.0, 75 * 100.0) / 100, 0 * getrand(0 * 100.0, 75 * 100.0) / 100, 0 * getrand(-75 * 100.0, 75 * 100.0) / 100},
		16);
}

#endif
