layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_spec;

in vec3 frag_normal;
in vec3 frag_position;

uniform vec4 object_color;

void main()
{    
	g_position = frag_position;

    g_normal = normalize(frag_normal);

    g_albedo_spec.rgb = vec3(0.0f, 1.0f, 0.0f);
   
    g_albedo_spec.a = object_color.w;
}