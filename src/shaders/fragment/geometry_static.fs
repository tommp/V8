layout (location = 0) out vec4 g_position;
layout (location = 1) out vec4 g_normal;
layout (location = 2) out vec4 g_albedo_spec;

in vec2 frag_tex_coord;
in vec3 frag_position;
in vec3 frag_normal;
flat in int instance;

struct Material {
    sampler2D diffuse;
    sampler2D normal;
    sampler2D reflectivity;
    sampler2D gloss;
}; 

uniform Material material;
uniform vec4 color_coeff[100];

layout (std140) uniform Plane_data
{
    vec2 plane_data;
};

float linearize_depth(float depth){
    float z = depth * 2.0 - 1.0;
    float linear_depth = (2.0 * plane_data.x * plane_data.y) / (plane_data.y + plane_data.x - z * (plane_data.y - plane_data.x)); 

    linear_depth /= plane_data.y;
    
    return linear_depth;
}

void main(){    
    g_position.xyz = frag_position;
    g_position.w = linearize_depth(gl_FragCoord.z);

    g_normal.xyz = texture(material.normal, frag_tex_coord).xyz;
    g_normal.w = texture(material.gloss, frag_tex_coord).r;

    g_albedo_spec.rgb = texture(material.diffuse, frag_tex_coord).rgb * color_coeff[instance].rgb;
    g_albedo_spec.a = texture(material.reflectivity, frag_tex_coord).r * color_coeff[instance].a;
}