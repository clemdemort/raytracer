#version 430 core
in vec3 aPos;   // the position variable has attribute position 0
out vec2 FragCoord;
void main() {
    gl_Position = vec4(aPos, 1.0);
    FragCoord = gl_Position.xy;
}
