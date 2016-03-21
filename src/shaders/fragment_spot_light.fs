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
uniform Spot_light light;

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

	vec3 light_direction = normalize(light.position - frag_position); 
 
	float diff = max(dot(normal, light_direction), 0.0);
	  
	vec3 reflect_direction = reflect(-light_direction, normal);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	float distance    = length(light.position - frag_position);
	float attenuation = 1.0f / (1.0f + light.linear * distance + light.quadratic * (distance * distance));   

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
			
	//HDR calculations
	const float gamma = 2.2;
	const float exposure = 1.0;

    vec3 hdrColor = ambient + diffuse + specular;
  
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    color = vec4(mapped, 1.0);
}