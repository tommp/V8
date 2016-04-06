layout (location = 0) out vec4 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_spec;
layout (location = 3) out vec4 g_bloom;

in vec3 frag_normal;
in vec3 frag_position;

uniform vec4 object_color;

layout (std140) uniform Plane_data
{
    vec2 plane_data;
};

float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    float linear_depth = (2.0 * plane_data.x * plane_data.y) / (plane_data.y + plane_data.x - z * (plane_data.y - plane_data.x)); 

    linear_depth /= plane_data.y;
    
    return linear_depth;
}

void main()
{    
	g_position.xyz = frag_position;

    g_position.w = linearize_depth(gl_FragCoord.z);

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