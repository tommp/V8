out float fragColor;

layout (std140) uniform Light_data
{
    vec2 screen_size;
};

uniform sampler2D SSAO_buffer;
const int BLUR_SIZE = 4;

void main() 
{
   vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
   vec2 texel_size = 1.0 / vec2(textureSize(SSAO_buffer, 0));
   float result = 0.0;
   for (int x = 0; x < BLUR_SIZE; ++x) 
   {
      for (int y = 0; y < BLUR_SIZE; ++y) 
      {
         vec2 offset = (vec2(-2.0) + vec2(float(x), float(y))) * texel_size;
         result += texture(SSAO_buffer, frag_tex_coord + offset).r;
      }
   }
 
   fragColor = result / float(BLUR_SIZE * BLUR_SIZE);
}