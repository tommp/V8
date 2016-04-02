layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_spec;
layout (location = 3) out vec4 g_bloom;

in vec3 frag_normal;
in vec3 frag_position;

uniform vec4 object_color;

void main()
{    
	g_position = frag_position;

    g_normal = normalize(frag_normal);

    g_albedo_spec.a = object_color.w;

    g_albedo_spec.rgb = object_color.xyz;

    float brightness = dot(object_color.xyz, vec3(0.2126, 0.7152, 0.0722));
    
    float factor = 0; 

    if(brightness > 1.0){
    	factor = 1;   
    }

    g_bloom = vec4(g_albedo_spec.rgb * factor, 1.0);
}