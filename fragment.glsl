/*
                        Raytracing shader
    - object detection
        float distance = detecthing(parameters ex; pos,rot,size)
*/

#version 430 core
//parameters
out vec4 FragColor;
in vec2 FragCoord;
//constants
float MAX_DIST = 1e10;

uniform vec3 CameraPos;
uniform vec3 CameraRot;
uniform vec2 iResolution;
uniform float Time;

uniform vec3 planeNormal;
uniform vec3 planePos;
uniform vec3 planeColour;

uniform int sphereNUM;

layout(std430, binding = 1) buffer spheresLayout
{
    float sphere_SSBO[];
};
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

vec3 unit_vector(vec3 v) {
    return v / v.length();
}

vec2 rotate2d(vec2 v, float a) {
	float sinA = sin(a);
	float cosA = cos(a);
	return vec2(v.x * cosA - v.y * sinA, v.y * cosA + v.x * sinA);
}

vec3 rotate3d(vec3 v, float a, float b) {

	float sinA = sin(a);
	float cosA = cos(a);
	float sinB = sin(b);
	float cosB = cos(b);
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
	return v*rotationA*rotationB;
}

vec4 SceneIntersection(vec3 rayDir, vec3 rayPos,vec4 BGColour)
{
    //this part will try to work out the colour of the ray by colliding it with the objects in the scene
    //--------------------------------------------------------------------------------------------------
    float hitDist = MAX_DIST;
    float dist = Plane(rayPos,rayDir);
    vec4 hitcolour = BGColour;
    if(dist < hitDist && dist > 0.0)
    {
        hitcolour = vec4(planeColour,1);
        hitDist = dist;
    }
    for(int i = 0; i < sphereNUM; i++){
        vec3 pos = vec3(sphere_SSBO[0+(i*9)],sphere_SSBO[1+(i*9)],sphere_SSBO[2+(i*9)]);
        float Radius = sphere_SSBO[3+(i*9)];
        float dist = Sphere(rayPos,rayDir,pos,Radius);
        if (dist < hitDist && dist > 0.0) {
            hitDist = dist;
            //vec3 N = unit_vector(vec3((rayPos + t*rayDir) - vec3(0,0,-1)));
            hitcolour = vec4(sphere_SSBO[4+(i*9)],sphere_SSBO[5+(i*9)],sphere_SSBO[6+(i*9)],1);
        }
    }
    return hitcolour;
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
	rayDir = normalize(rotate3d(rayDir,CameraRot.y,CameraRot.x));

	//coding the sky
	//--------------
	vec3 horizon = vec3(0.5,0.5,1);
	vec3 up = vec3(0.25,0.25,0.75);
	vec3 down = vec3(0.05,0.05,0.15);
	vec3 col = abs(rayDir.y * down) + abs((1 - rayDir.y) * up) + abs((0.1/rayDir.y) * horizon)+ abs((0.75/rayDir.y + 0.5f) * horizon*0.15);
	FragColor = vec4(col, 1.0);
    //-----------------------------
    FragColor = SceneIntersection(rayDir,rayPos,FragColor);

}
