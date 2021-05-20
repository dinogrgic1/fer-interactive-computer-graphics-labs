#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

uniform mat4 matProjection;
uniform mat4 matView;

void main() {

    vec4 v_x = matProjection * matView * gl_in[0].gl_Position;
    vec4 v_x1 = matProjection * matView * gl_in[1].gl_Position;
    vec4 v_x2 = matProjection * matView * gl_in[2].gl_Position;

    vec2 edge1 = ((v_x1.xy / v_x1.w) - (v_x.xy / v_x.w));
    vec2 edge2 = ((v_x2.xy / v_x2.w) - (v_x.xy / v_x.w));

    float area = (edge1.x)*(edge2.y) - (edge1.y)*(edge2.x); // we only care about sign of area

    if(area < 0.0f) {
        return;
    }

    gl_Position = v_x;
    EmitVertex();
    gl_Position = v_x1;
    EmitVertex();
    gl_Position = v_x2;
    EmitVertex();
    EndPrimitive();
}