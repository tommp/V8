struct Spot_light {
	vec3 position;
	vec3 direction;

	float cut_off;
	float outer_cut_off;
  
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
uniform Spot_light spot_light;

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
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;

	vec3 light_direction = normalize(spot_light.position - frag_position); 
 
	float diff = max(dot(normal, light_direction), 0.0);
	  
	vec3 reflect_direction = reflect(-light_direction, normal);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	float distance    = length(spot_light.position - frag_position);
	float attenuation = 1.0f / (1.0f + spot_light.linear * distance + spot_light.quadratic * (distance * distance));   

	vec3 ambient = spot_light.ambient * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 diffuse = spot_light.diffuse * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);
	vec3 specular = spot_light.specular * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
	
	float theta = dot(light_direction, normalize(-spot_light.direction)); 
	float epsilon = (spot_light.cut_off - spot_light.outer_cut_off);
	float intensity = clamp((theta - spot_light.outer_cut_off) / epsilon, 0.0, 1.0);

	diffuse  *= intensity;
	specular *= intensity;
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   
			
	color = vec4(ambient + diffuse + specular, 1.0);
}