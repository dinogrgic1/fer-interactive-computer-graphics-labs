#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

uniform mat4 matProjection;
uniform mat4 matView;
uniform vec3 eyeView;

out fData 
{
    vec3 normal;
    vec4 color;
} frag;

void main() {
    vec3 v_x = vec3(gl_in[0].gl_Position.xyz);
    vec3 v_x1 = vec3(gl_in[1].gl_Position.xyz);
    vec3 v_x2 = vec3(gl_in[2].gl_Position.xyz);

    vec3 c = (v_x + v_x1 + v_x2) / 3;
    vec3 e = eyeView - c;
    
    vec3 normal = cross((v_x1 - v_x), (v_x2 - v_x));
    float res = dot(normal, e);

    frag.color = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    frag.normal = normal;
    gl_Position = matProjection * matView * vec4(v_x, 1.0f);
    EmitVertex();
    frag.color = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    frag.normal = normal;
    gl_Position = matProjection * matView * vec4(v_x1, 1.0f);
    EmitVertex();
    frag.color = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    frag.normal = normal;
    gl_Position = matProjection * matView * vec4(v_x2, 1.0f);
    EmitVertex();
    EndPrimitive();
}