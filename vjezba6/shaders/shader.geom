#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

uniform mat4 matProjection;
uniform mat4 matView;

void main() {
    gl_Position = matProjection * matView * vec4(gl_in[0].gl_Position.xyz, 1.0f);
    EmitVertex();
    gl_Position = matProjection * matView * vec4(gl_in[1].gl_Position.xyz, 1.0f);
    EmitVertex();
    gl_Position = matProjection * matView * vec4(gl_in[2].gl_Position.xyz, 1.0f);
    EmitVertex();
    EndPrimitive();
}