#define MAX_NR_DIRECTIONAL_LIGHTS 1
#define MAX_NR_POINT_LIGHTS 15
#define MAX_NR_SPOT_LIGHTS 1

struct Directional_light {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point_light {
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Spot_light {
	vec3 position;
	vec3 direction;

	float cut_off;
	float outer_cut_off;
  
	float constant;
	float linear;
	float quadratic;
  
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;       
};

out vec4 color;

in vec2 frag_tex_coord;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

uniform vec3 view_position;

uniform Directional_light directional_lights[MAX_NR_DIRECTIONAL_LIGHTS];
uniform Point_light point_lights[MAX_NR_POINT_LIGHTS];
uniform Spot_light spot_lights[MAX_NR_SPOT_LIGHTS];

vec3 calc_directional_light(Directional_light light, vec3 frag_normal, vec3 view_direction);
vec3 calc_point_light(Point_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction);
vec3 calc_spot_light(Spot_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction);

int shininess = 32;

void main()
{    
	// Properties

	vec3 norm = normalize(texture(g_normal, frag_tex_coord).rgb);
	vec3 view_direction = normalize(view_position - texture(g_position, frag_tex_coord).rgb);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;

	for(int i = 0; i < MAX_NR_DIRECTIONAL_LIGHTS; i++){
		result += calc_directional_light(directional_lights[i], norm, view_direction);
	}

	for(int i = 0; i < MAX_NR_POINT_LIGHTS; i++){
		result += calc_point_light(point_lights[i], norm, frag_position, view_direction);    
	}

	for(int i = 0; i < MAX_NR_SPOT_LIGHTS; i++){
		result += calc_spot_light(spot_lights[i], norm, frag_position, view_direction);     
	}
	
	color = vec4(result, 1.0);
}

vec3 calc_directional_light(Directional_light light, vec3 frag_normal, vec3 view_direction)
{
	vec3 light_direction = normalize(-light.direction);

	float diff = max(dot(frag_normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, frag_normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 diffuse = light.diffuse * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 specular = light.specular * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
	return (ambient + diffuse + specular);
}

vec3 calc_point_light(Point_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - frag_position);

	float diff = max(dot(frag_normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, frag_normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	float distance = length(light.position - frag_position);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 ambient = light.ambient * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 diffuse = light.diffuse * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 specular = light.specular * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calc_spot_light(Spot_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - frag_position); 
 
	float diff = max(dot(frag_normal, light_direction), 0.0);
	  
	vec3 reflect_direction = reflect(-light_direction, frag_normal);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	float distance    = length(light.position - frag_position);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

	vec3 ambient = light.ambient * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 diffuse = light.diffuse * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 specular = light.specular * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
	
	float theta = dot(light_direction, normalize(-light.direction)); 
	float epsilon = (light.cut_off - light.outer_cut_off);
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	diffuse  *= intensity;
	specular *= intensity;
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   
			
	return (ambient + diffuse + specular);  
}