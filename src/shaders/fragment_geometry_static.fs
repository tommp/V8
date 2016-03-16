layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_spec;

in vec2 frag_tex_coord;
in vec3 frag_position;
in vec3 frag_normal;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
}; 

uniform Material material;

void main()
{    
    g_position = frag_position;

    g_normal = normalize(frag_normal);

    g_albedo_spec.rgb = texture(material.diffuse, frag_tex_coord).rgb;
   
    g_albedo_spec.a = texture(material.specular, frag_tex_coord).r;
}