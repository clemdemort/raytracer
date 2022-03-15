#version 430 core
out vec4 FragColor;
in vec2 FragCoord;
uniform vec3 CameraPos;
uniform vec3 CameraRot;
uniform vec2 iResolution;
uniform float Time;
uniform int sphereNUM;

layout(std430, binding = 1) buffer planesLayout
{
    float plane_SSBO[];
};

layout(std430, binding = 2) buffer spheresLayout
{
    float sphere_SSBO[];
};
//defining a sphere
struct sphere
{
    vec3 Pos;
    vec3 colour;
    float Radius;
};

//defining a sphere
sphere ball = sphere(vec3(sphere_SSBO[0],sphere_SSBO[1],sphere_SSBO[2]),vec3(sphere_SSBO[4],sphere_SSBO[5],sphere_SSBO[6]),sphere_SSBO[3]);

struct plane
{
    vec3 Pos;
    vec3 normal;
    vec3 colour;
};

plane ground = plane(vec3(plane_SSBO[0],plane_SSBO[1],plane_SSBO[2]),vec3(plane_SSBO[3],plane_SSBO[4],plane_SSBO[5]),vec3(plane_SSBO[6],plane_SSBO[7],plane_SSBO[8]));

//ray sphere collision
float hit_sphere(int IDX,vec3 rayPos, vec3 rayDir) {

    vec3 pos = vec3(sphere_SSBO[0+(IDX*9)],sphere_SSBO[1+(IDX*9)],sphere_SSBO[2+(IDX*9)]);
    float Radius = sphere_SSBO[3+(IDX*9)];

    vec3 oc = rayPos - pos;
    float a = dot(rayDir, rayDir);
    float b = 2.0 * dot(oc, rayDir);
    float c = dot(oc, oc) - Radius*Radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        //ray doesn't intersect with the ball
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}

float hit_plane(plane ground,vec3 rayPos, vec3 rayDir)
{
    // assuming vectors are all normalized
    float t;
    float denom = dot(ground.normal, rayDir);
    if (denom < 1e-6) {
        vec3 discriminant = ground.Pos - rayPos;
        t = dot(discriminant, ground.normal) / denom;
        return t;
    }
    return -1.0;
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
	rayDir = rotate3d(rayDir,CameraRot.y,CameraRot.x);
	vec3 horizon = vec3(0.5,0.5,1);
	vec3 up = vec3(0.25,0.25,0.75);
	vec3 down = vec3(0.05,0.05,0.15);
	vec3 col = abs(rayDir.y * down) + abs((1 - rayDir.y) * up) + abs((0.1/rayDir.y) * horizon)+ abs((0.75/rayDir.y + 0.5f) * horizon*0.15);
	FragColor = vec4(col, 1.0);
    //-----------------------------

    //this part will try to work out the colour of the ray


    float t = hit_plane(ground,rayPos,rayDir);
    if(t > 0.0)
    {
        FragColor = vec4(ground.colour,1);
    }
    for(int i = 0; i < sphereNUM; i++){
        t = hit_sphere(i,rayPos,rayDir);
        if (t > 0.0) {
            vec3 N = unit_vector(vec3((rayPos + t*rayDir) - vec3(0,0,-1)));
            FragColor = vec4(sphere_SSBO[4+(i*9)],sphere_SSBO[5+(i*9)],sphere_SSBO[6+(i*9)],1);
        }
    }
}
