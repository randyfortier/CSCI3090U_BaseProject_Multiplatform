#version 110

attribute vec3 position;

uniform mat4 u_MVPMatrix;

void main() {
   gl_Position = u_MVPMatrix * vec4(position, 1.0);
}
