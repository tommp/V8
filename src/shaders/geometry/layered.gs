layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

flat in int instance[];

uniform vec3 world_position;

void main(){
    gl_Layer = instance[0];

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}