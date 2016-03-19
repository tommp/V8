out vec4 frag_color;

uniform sampler2D g_bloom;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

layout (std140) uniform Light_data
{
    vec2 screen_size;
};

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(g_bloom, 0); // gets size of single texel
    vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
    vec3 result = texture(g_bloom, frag_tex_coord).rgb * weight[0];

    for(int i = 1; i < 5; ++i){
        result += texture(g_bloom, frag_tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(g_bloom, frag_tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
    
    frag_color = vec4(result, 1.0);
}