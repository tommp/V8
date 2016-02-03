//Static for now, until engine shader compilation is more flexible 
#define NR_DIRECTIONAL_LIGHTS 1
#define NR_POINT_LIGHTS 50
#define NR_SPOT_LIGHTS 10

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
}; 

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

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_tex_coord;

out vec4 color;

uniform Material material;

uniform vec3 view_position;
uniform Directional_light directional_lights[NR_DIRECTIONAL_LIGHTS];
uniform Point_light point_lights[NR_POINT_LIGHTS];
uniform Spot_light spot_lights[NR_SPOT_LIGHTS];

// Function prototypes
vec3 CalcDirectional_light(Directional_light light, vec3 frag_normal, vec3 view_direction);
vec3 CalcPoint_light(Point_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction);

void main()
{    
	// Properties
	vec3 norm = frag_normalize(frag_normal);
	vec3 view_direction = frag_normalize(view_position - frag_position);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	// Phase 1: Directional lights
	for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++){
		vec3 result += calc_directional_light(Directional_light, norm, view_direction);
	}
	// Phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calc_point_light(point_lights[i], norm, frag_position, view_direction);    
	// Phase 3: Spot light
	for(int i = 0; i < NR_SPOT_LIGHTS; i++)
		result += calc_spot_light(spot_lights[i], norm, frag_position, view_direction);  
	// result += CalcSpotLight(spotLight, norm, frag_position, view_direction);    
	
	color = vec4(result, 1.0);
}

// Calculates the color when using a directional light.
vec3 calc_cirectional_light(Directional_light light, vec3 frag_normal, vec3 view_direction)
{
	vec3 light_direction = frag_normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(frag_normal, light_direction), 0.0);
	// Specular shading
	vec3 reflect_direction = reflect(-light_direction, frag_normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	// Combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_tex_coord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_tex_coord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_tex_coord));
	return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 calc_point_light(Point_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction)
{
	vec3 light_direction = frag_normalize(light.position - frag_position);
	// Diffuse shading
	float diff = max(dot(frag_normal, light_direction), 0.0);
	// Specular shading
	vec3 reflect_direction = reflect(-light_direction, frag_normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position - frag_position);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_tex_coord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_tex_coord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_tex_coord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 calc_spot_light(Spot_light light, vec3 frag_normal, vec3 frag_position, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - frag_position); 

	// Diffuse    
	float diff = max(dot(frag_normal, light_direction), 0.0);
	  
	// Specular
	vec3 reflect_direction = reflect(-light_direction, frag_normal);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

	// Attenuation
	float distance    = length(light.position - frag_position);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_tex_coord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_tex_coord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_tex_coord));
	
	// Spotlight (soft edges)
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