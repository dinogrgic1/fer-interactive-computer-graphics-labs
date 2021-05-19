#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 matProjection;
uniform mat4 matView;
uniform vec3 eyeView;

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}