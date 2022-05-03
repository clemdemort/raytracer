/*
                        Raytracing shader
    - object detection
        distance(+normal) = detecthing(parameters ex; pos,rot,size)
        if(otherintersectiondistance < actual distance)
            change colour to that of this object
            setS the normal of this pixel/ray
            actual distance = otherintersectiondistance
    - object shading
        colour = colour + shasow rays + dot product shading (with a bias to make things look better)

    - voxel detection
        get raypos relative to voxel object
        substract position of object to raypos
        raymarch the voxel data using DDA
*/

#version 430 core
//parameters
out vec4 FragColor;
in vec2 FragCoord;
//constants
float MAX_DIST = 10000000;
float bias = 0.0001;        //too small = shadow acne , too big = inacurate shadows.
uniform vec3 CameraPos;
uniform vec3 CameraRot;
uniform vec2 iResolution;
uniform float Time;
uniform float Rand;
uniform vec3 planeNormal;   //the plane is passed as a uniform because who needs more than one anyways????
uniform vec3 planePos;
uniform vec3 planeColour;
uniform vec3 sunDir;
uniform int getNormals;     //debugging stuff
uniform int sphereNUM;      //variables to interract with the SSBOs
uniform int cubeNUM;
uniform int voxelNUM;
layout(r8ui, binding = 4) uniform uimage3D voxATLAS;//gets the binded texture

//global variables

vec3 normal;
vec2 HDistance;

layout(std430, binding = 1) buffer spheresLayout
{
    float sphere_SSBO[];
};

layout(std430, binding = 2) buffer cubesLayout
{
    float cube_SSBO[];
};

layout(std430, binding = 3) buffer voxelsLayout
{
    float voxel_SSBO[];
};

vec2 rotate2d(vec2 v, float a) {
	float sinA = sin(a);
	float cosA = cos(a);
	return vec2(v.x * cosA - v.y * sinA, v.y * cosA + v.x * sinA);
}

vec3 rotate3d(vec3 v, float a, float b,float c) {

	float sinA = sin(a);
	float cosA = cos(a);
	float sinB = sin(b);
	float cosB = cos(b);
    float sinC = sin(c);
	float cosC = cos(c);
	mat3 rotationA = mat3(
	1               ,  0            ,    0         ,
    0               ,  cosA         ,    -sinA     ,
    0               ,  sinA         ,    cosA
	);
	mat3 rotationB = mat3(
	cosB            ,  0            ,    sinB      ,
    0               ,  1            ,    0         ,
    -sinB           ,  0            ,    cosB
	);
    mat3 rotationC = mat3(
	cosC            ,  -sinC        ,     0        ,
    sinC            ,  cosC         ,     0        ,
    0               ,  0            ,     1
	);
	return v*rotationA*rotationB*rotationC;
}

vec3 invrotate3d(vec3 v, float a, float b,float c) {

	float sinA = sin(a);
	float cosA = cos(a);
	float sinB = sin(b);
	float cosB = cos(b);
    float sinC = sin(c);
	float cosC = cos(c);
	mat3 rotationA = mat3(
	1               ,  0            ,    0         ,
    0               ,  cosA         ,    -sinA     ,
    0               ,  sinA         ,    cosA
	);
	mat3 rotationB = mat3(
	cosB            ,  0            ,    sinB      ,
    0               ,  1            ,    0         ,
    -sinB           ,  0            ,    cosB
	);
    mat3 rotationC = mat3(
	cosC            ,  -sinC        ,     0        ,
    sinC            ,  cosC         ,     0        ,
    0               ,  0            ,     1
	);
	return v*inverse(rotationA*rotationB*rotationC);
}



//ray box intersection
vec2 Cube(vec3 raypos, vec3 raydir,vec3 pos, vec3 boxSize,vec3 rot)
{
    vec3 rayDir = rotate3d(raydir,rot.x,rot.y,rot.z);
    vec3 rayPos = rotate3d(raypos,rot.x,rot.y,rot.z);
    vec3 m = 1.0/rayDir;
    vec3 n = m*(rayPos-rotate3d(pos,rot.x,rot.y,rot.z));
    vec3 k = abs(m)*boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );//    far intersection which we "might" need in the future.
    if( tN>=tF || tF<0.0) return vec2(-1.0); // no intersection
    normal = invrotate3d(-sign(rayDir)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz),rot.x,rot.y,rot.z);
    return vec2(tN,tF);
}

//ray sphere collision
float Sphere(vec3 rayPos,vec3 rayDir, vec3 pos, float Radius )
{
    vec3 oc = rayPos - pos;
    float b = dot( oc, rayDir );
    float c = dot( oc, oc ) - Radius*Radius;
    float h = b*b - c;
    if( h<0.0 ) return -1.0; // no intersection
    h = sqrt( h );
    return -b-h;    //we take the closest intersection the furthest would be -b+h
}

//ray plane intersection
float Plane(vec3 rayPos,vec3 rayDir) {
    return -(dot(rayPos,planeNormal.xyz))/dot(rayDir,planeNormal.xyz);
}
//gets the voxel at the index's position
uint getVoxel(ivec3 Index){
    return imageLoad(voxATLAS,Index).r;
}


//if this function touched something output the colour of the touched voxel
//if this function did NOT touch something output an empty vector
//will output vec2(float (distance),float (material))
//WARNING: currently the function calculates incorrectly: distance
vec2 Voxels(vec3 RayDir, vec3 RayPos,vec3 pos, vec3 boxSize,vec3 rot,ivec3 listOffset,ivec3 SampleSize, vec2 Distance){//not done
    //         getting point of intersection                 correcting the center of the box
    vec3 POI;
    vec3 rayPos = rotate3d(RayPos,rot.x,rot.y,rot.z);
    vec3 rayDir = rotate3d(RayDir,rot.x,rot.y,rot.z);
    if(Distance.x < 0 && Distance.y > 0)//if the camera is inside the object then POI will be equal to the position of the camera + boxpos
    {
        POI = SampleSize*((rayPos)-rotate3d(pos,rot.x,rot.y,rot.z)+(boxSize))/((boxSize)*2);
    }else{
        POI = SampleSize*((rayPos+(rayDir*Distance.x))-rotate3d(pos,rot.x,rot.y,rot.z)+(boxSize))/((boxSize)*2)+rotate3d(normal,rot.x,rot.y,rot.z)*bias;
    }
    ivec3 VoxPos = ivec3(floor(POI));
	vec3 deltaDist = abs(vec3(length(rayDir)) / (rayDir));
	ivec3 rayStep = ivec3(sign(rayDir));
	vec3 sideDist = (sign(rayDir) * (vec3(VoxPos) - (POI)) + (sign(rayDir) * 0.5) + 0.5) * deltaDist;
	bvec3 mask;
	bool touched = false;
	bool insidebox = true;
	while(touched == false && insidebox)
	{
		if (sideDist.x < sideDist.y) {
				if (sideDist.x < sideDist.z) {
					sideDist.x += deltaDist.x;
					VoxPos.x += rayStep.x;
					mask = bvec3(true, false, false);
				}
				else {
					sideDist.z += deltaDist.z;
					VoxPos.z += rayStep.z;
					mask = bvec3(false, false, true);
				}
			}
			else {
				if (sideDist.y < sideDist.z) {
					sideDist.y += deltaDist.y;
					VoxPos.y += rayStep.y;
					mask = bvec3(false, true, false);
				}
				else {
					sideDist.z += deltaDist.z;
					VoxPos.z += rayStep.z;
					mask = bvec3(false, false, true);
				}
			}
			if(VoxPos.x >= 0 && VoxPos.x <= SampleSize.x && VoxPos.y >= 0 && VoxPos.y <= SampleSize.y && VoxPos.z >= 0 && VoxPos.z <= SampleSize.z){//is there an easier way to do this?
                insidebox = true;
                uint voxel = getVoxel(ivec3(VoxPos+listOffset));
                if(voxel != 0.0)//something was touched
                {
                    normal = invrotate3d(vec3(-sign(rayDir)*vec3((mask.xyz))),rot.x,rot.y,rot.z);
                    float t = dot(sideDist - deltaDist, vec3(mask));// thanks Kpreid for the very smart piece of code!
                    float voxDist = distance(rayPos+(rayDir*Distance.x),((POI+rayDir*t)*boxSize*2/SampleSize) + rotate3d(pos,rot.x,rot.y,rot.z) - (boxSize));
                    float hitDistance = Distance.x + voxDist;
                    return vec2(hitDistance,voxel);
                }
			}else{
                insidebox = false;
			}
    }
    return vec2(0);
}

vec4 renderPass(vec3 rayDir, vec3 rayPos,vec4 oldColour)
{
    //this part will try to work out the properties of the ray by colliding it with the objects in the scene
    //--------------------------------------------------------------------------------------------------
    HDistance.x = MAX_DIST;
    //rayprop: vec3(R,G,B), float(did_the_ray_hit?)->1 = it did hit -> -1 = it didn't.
    vec4 rayProp = vec4(oldColour.xyz,-1); //the ray didn't hit anything yet so rayprop.z = -1;
    //ray plane SceneIntersection
    float dist = Plane(rayPos,rayDir);
    if(dist < HDistance.x && dist > 0.0)
    {
        vec3 checkers = (dist*rayDir+rayPos)/50;
        if(int(checkers.x+MAX_DIST) % 2 == int(checkers.z+MAX_DIST) % 2 ){
            rayProp = vec4(planeColour,1);
        }else{
            rayProp = vec4(planeColour*0.5,1);
        }
        HDistance.x = dist;
        normal = planeNormal;
    }
    for(int i = 0; i < sphereNUM; i++){
        vec3 pos = vec3(sphere_SSBO[0+(i*9)],sphere_SSBO[1+(i*9)],sphere_SSBO[2+(i*9)]);
        float Radius = sphere_SSBO[3+(i*9)];
        float dist = Sphere(rayPos,rayDir,pos,Radius);
        if (dist < HDistance.x && dist > 0.0) {
            HDistance.x = dist;
            rayProp = vec4(sphere_SSBO[4+(i*9)],sphere_SSBO[5+(i*9)],sphere_SSBO[6+(i*9)],1);
            normal = normalize(rayPos+(rayDir*dist) - pos);
        }
    }
    for(int i = 0; i < cubeNUM; i++){
        vec3 pos = vec3(cube_SSBO[0+(i*14)],cube_SSBO[1+(i*14)],cube_SSBO[2+(i*14)]);
        vec3 size = vec3(cube_SSBO[3+(i*14)],cube_SSBO[4+(i*14)],cube_SSBO[5+(i*14)]);
        vec3 rotation = vec3(cube_SSBO[6+(i*14)],cube_SSBO[7+(i*14)],cube_SSBO[8+(i*14)]);
        vec3 temp = normal;
        vec2 param = Cube(rayPos,rayDir,pos,size,rotation);
        if (param.x < HDistance.x && param.x > 0.0) {
            HDistance.x = param.x;
            HDistance.y = param.y;
            rayProp = vec4(cube_SSBO[9+(i*14)],cube_SSBO[10+(i*14)],cube_SSBO[11+(i*14)],1);
            temp = normal;
        }
        normal = temp;
    }
    for(int i = 0; i < voxelNUM; i++){
        vec3 pos = vec3(voxel_SSBO[0+(i*15)],voxel_SSBO[1+(i*15)],voxel_SSBO[2+(i*15)]);
        vec3 boxSize = vec3(voxel_SSBO[3+(i*15)],voxel_SSBO[4+(i*15)],voxel_SSBO[5+(i*15)]);
        vec3 rotation = vec3(voxel_SSBO[6+(i*15)],voxel_SSBO[7+(i*15)],voxel_SSBO[8+(i*15)]);
        ivec3 listOffset = ivec3(voxel_SSBO[9+(i*15)],voxel_SSBO[10+(i*15)],voxel_SSBO[11+(i*15)]);
        ivec3 SampleSize = ivec3(voxel_SSBO[12+(i*15)],voxel_SSBO[13+(i*15)],voxel_SSBO[14+(i*15)]);
        vec3 temp = normal;
        vec2 param = Cube(rayPos,rayDir,pos,boxSize,rotation);
            //distance check                         //checks if the raypos is already inside the bounding box
        if ((param.x < HDistance.x && param.x > 0.0) || (0>=param.x && 0<param.y) ){
            param = Voxels(rayDir,rayPos,pos,boxSize,rotation,listOffset,SampleSize, param);
            if (param.x < HDistance.x && param.x > 0.0){
                HDistance.x = param.x;
                //HDistance.y = param.y;
                rayProp = vec4(1);     //for now a quick way to visualize the individual voxels
                temp = normal;
            }
        }
        normal = temp;
    }
    return rayProp;
}

vec3 SceneIntersection(vec3 rayDir, vec3 rayPos,vec4 BGColour)
{
    vec4 rayCol = renderPass(rayDir,rayPos,BGColour);
    if(rayCol.w > 0) return rayCol.xyz;
}
vec4 ShadowRays(vec3 rayDir, vec3 rayPos,vec4 oldColour)
{
    //this part will collide yet another time with the scene to determine the shadows
    //-------------------------------------------------------------------------------
    vec3 temp = normal;//otherwise we would have 3D shadows which while cool arent realistic XD
    if(renderPass(rayDir,rayPos,oldColour).w == 1)
    {
        normal = temp;
        return vec4(oldColour.x,oldColour.y,oldColour.z,8)/8.0;
    }
    return oldColour;
}
vec3 skyColor(vec3 rayDir )
{
    vec3 sundir = normalize(sunDir);

    float yd = min(rayDir.y, 0);
    rayDir.y = max(rayDir.y, 0);

    vec3 col = vec3(0);

    col += vec3(0.4, 0.4 - exp( -rayDir.y*20.0 )*0.15, 0) * exp(-rayDir.y*9.0); // Red / Green
    col += vec3(0.3, 0.5, 0.6) * (1.0 - exp(-rayDir.y*8.0) ) * exp(-rayDir.y*0.9) ; // Blue

    col = mix(col*1.2, vec3(0.3),  1.0-exp(yd*100.0)); // Fog

    col += vec3(1.0, 0.8, 0.55) * pow( max(dot(rayDir,sundir),0.), 150. ) * 0.6; // Sun
    col += pow(max(dot(rayDir, sundir),0.0), 150.0) *0.15;

    return col;
}

void main()
{
    //defining some base properties
    float FOV = 90;
    vec2 screenPos = rotate2d(vec2(FragCoord.x,FragCoord.y * (iResolution.y / iResolution.x)),CameraRot.z);
	vec3 cameraDir = vec3(0.0, 0.0, (180/FOV)-1);
	vec3 cameraPlaneU = vec3(1.0, 0.0, 0.0);
	vec3 cameraPlaneV = vec3(0.0, 1.0, 0.0);
	vec3 rayDir = cameraDir + screenPos.x * cameraPlaneU + screenPos.y * cameraPlaneV;
	vec3 rayPos = CameraPos;
	rayDir = normalize(rotate3d(rayDir,CameraRot.y,CameraRot.x,0));

	//coding the sky
	//--------------
    normal = sunDir;       //this is done otherwise the sky would be black if we use it in a normal calculation
    FragColor.xyz = skyColor(rayDir);

    //-----------------------------
    vec3 sceneParam = SceneIntersection(rayDir,rayPos,FragColor);
    FragColor.xyz = sceneParam;
    if(getNormals == 1){                //to help us visualize normals
        FragColor.xyz = ((1+normal.xyz)*0.5);
    }else{                              //if we are visualizing normals we arent interested in shadows.
        rayPos = rayPos+(rayDir*HDistance.x)+(normal*bias*HDistance.x); //we need some variable bias to prevent "shadow acne"
        rayDir = normalize(sunDir);
        //FragColor.xyz = renderPass(normal,rayPos,FragColor).xyz; //this makes the ray bounce but is still work in progress
        //this is a bit of a hack, but im basically averaging the shadows and the original colour to make the shadows smoother.
        FragColor = (vec4(sceneParam/2,0)+(ShadowRays(rayDir,rayPos,FragColor))*(dot(normal,sunDir)))/1.5;
    }
}
