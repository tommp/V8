struct Point_light {
	vec3 position;
	
	float radius;
	
	vec3 color;
	vec3 color_components;
};

out vec4 color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D SSAO_buffer;

uniform vec3 view_position;
uniform Point_light light;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

const int shininess = 32;

void main()
{    
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).rgb);
	vec3 view_direction = normalize(view_position - texture(g_position, frag_tex_coord).rgb);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;
	float ambient_occlusion = texture(SSAO_buffer, frag_tex_coord).r;

	vec3 light_direction = normalize(light.position - frag_position);

	float diff = max(dot(normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	float distance = length(light.position - frag_position);

	float attenuation = clamp(1.0 - distance/light.radius, 0.0, 1.0);

	attenuation *= attenuation;

	vec3 ambient = (light.color * light.color_components.x) * vec3(texture(g_albedo_spec, frag_tex_coord).rgb) * vec3(0.3 * ambient_occlusion);
	vec3 diffuse = (light.color * light.color_components.y) * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 specular = (light.color * light.color_components.z) * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
  
    color = vec4(ambient + diffuse + specular, 1.0);
}