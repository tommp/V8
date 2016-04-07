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

uniform vec3 view_position;
uniform Point_light light;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

const int shininess = 32;

void main()
{    
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
	vec3 frag_position = texture(g_position, frag_tex_coord).xyz;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).xyz);
	float distance = length(light.position - frag_position);
	vec3 albedo_color = texture(g_albedo_spec, frag_tex_coord).rgb;

	vec3 view_direction = normalize(view_position - frag_position);
	vec3 light_direction = normalize(light.position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, normal);

	float diff = max(dot(normal, light_direction), 0.0);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	vec3 ambient = (light.color * light.color_components.x) * albedo_color;

	vec3 diffuse = (light.color * light.color_components.y) * diff * albedo_color;

	vec3 specular = (light.color * light.color_components.z) * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);

	float attenuation = clamp(1.0 - distance/light.radius, 0.0, 1.0);
	attenuation *= attenuation;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    color = vec4(ambient + diffuse + specular, 1.0);
}