out vec4 frag_color;

in vec2 frag_tex_coord;

uniform sampler2D color;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(color, 0); // gets size of single texel
    vec3 result = texture(color, frag_tex_coord).rgb * weight[0];

    for(int i = 1; i < 5; ++i){
        result += texture(color, frag_tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(color, frag_tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
    
    frag_color = vec4(result, 1.0);
}