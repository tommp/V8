out vec4 AA_buffer;

uniform sampler2D colortexture;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

const float FXAA_span_max = 8.0;
const float FXAA_reduce_mul = 1.0/8.0;
const float FXAA_reduce_min = (1.0/128.0);

void main(){
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;

	vec3 rgb_NW = textureOffset(colortexture, frag_tex_coord, ivec2(-1,-1)).xyz;
    vec3 rgb_NE = textureOffset(colortexture, frag_tex_coord, ivec2(1,-1)).xyz;
    vec3 rgb_SW = textureOffset(colortexture, frag_tex_coord, ivec2(-1,1)).xyz;
    vec3 rgb_SE = textureOffset(colortexture, frag_tex_coord, ivec2(1,1)).xyz;
    vec3 rgb_C  = texture(colortexture, frag_tex_coord).xyz;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float luma_NW = dot(rgb_NW, luma);
    float luma_NE = dot(rgb_NE, luma);
    float luma_SW = dot(rgb_SW, luma);
    float luma_SE = dot(rgb_SE, luma);
    float luma_C  = dot(rgb_C,  luma);
	
	float luma_min = min(luma_C, min(min(luma_NW, luma_NE), min(luma_SW, luma_SE)));
    float luma_max = max(luma_C, max(max(luma_NW, luma_NE), max(luma_SW, luma_SE)));

    vec2 dir; 
    dir.x = -((luma_NW + luma_NE) - (luma_SW + luma_SE));
    dir.y =  ((luma_NW + luma_SW) - (luma_NE + luma_SE));

    float dir_reduce = max((luma_NW + luma_NE + luma_SW + luma_SE) * (0.25 * FXAA_reduce_mul), FXAA_reduce_min);

    float rcp_dir_min = 1.0/(min(abs(dir.x), abs(dir.y)) + dir_reduce);
    dir = min(vec2( FXAA_span_max,  FXAA_span_max), 
          max(vec2(-FXAA_span_max, -FXAA_span_max), 
          dir * rcp_dir_min)) * 1/screen_size;

    vec3 result_1 = (1.0/2.0) * (
        texture(colortexture, frag_tex_coord + dir * (1.0/3.0 - 0.5)).xyz +
        texture(colortexture, frag_tex_coord + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 result_2 = result_1 * (1.0/2.0) + (1.0/4.0) * (
        texture(colortexture, frag_tex_coord + dir * (0.0/3.0 - 0.5)).xyz +
        texture(colortexture, frag_tex_coord + dir * (3.0/3.0 - 0.5)).xyz);

    float luma_res = dot(result_2, luma);

    AA_buffer = vec4(result_2, texture(colortexture, frag_tex_coord).w);

    if((luma_res < luma_min) || (luma_res > luma_max)){
        AA_buffer = vec4(result_1, texture(colortexture, frag_tex_coord).w);
    } 
}