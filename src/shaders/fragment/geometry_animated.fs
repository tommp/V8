layout (location = 0) out vec4 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_spec;
layout (location = 3) out vec4 g_bloom;

in vec2 frag_tex_coord;
in vec3 frag_position;
in vec3 frag_normal;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
}; 

uniform Material material;

layout (std140) uniform Plane_data
{
    float near_plane;
    float far_plane;
};


float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));    
}

void main()
{    
    g_position.xyz = frag_position;

    g_position.a = linearize_depth(gl_FragCoord.z);

    g_normal = normalize(frag_normal);

    g_albedo_spec.a = texture(material.specular, frag_tex_coord).r;

    g_albedo_spec.rgb = texture(material.diffuse, frag_tex_coord).rgb;

    float brightness = dot(g_albedo_spec.rgb, vec3(0.2126, 0.7152, 0.0722));
    
    float factor = 0; 

    if(brightness > 1.0){
    	factor = 1;   
    }

    g_bloom = vec4(g_albedo_spec.rgb * factor, 1.0);
}