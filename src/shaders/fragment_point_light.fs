struct Point_light {
	vec3 position;
	
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

uniform vec3 view_position;
uniform Point_light point_light;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

int shininess = 32;

void main()
{    
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).rgb);
	vec3 view_direction = normalize(view_position - texture(g_position, frag_tex_coord).rgb);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;

	vec3 light_direction = normalize(point_light.position - frag_position);

	float diff = max(dot(normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	float distance = length(point_light.position - frag_position);
	float attenuation = 1.0f / (1.0f + point_light.linear * distance + point_light.quadratic * (distance * distance));    

	vec3 ambient = point_light.ambient * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 diffuse = point_light.diffuse * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 specular = point_light.specular * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	color = vec4(ambient + diffuse + specular, 1.0);
}