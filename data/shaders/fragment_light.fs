out vec4 color;

in vec2 frag_tex_coord;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

void main()
{    
    color = vec4((texture(g_normal, frag_tex_coord).rgb), 1.0f) * 0.0001f;
}