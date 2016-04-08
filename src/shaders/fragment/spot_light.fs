struct Spot_light {
	vec3 position;
	vec3 direction;

	float cut_off;
	float outer_cut_off;
  
	float radius;

	vec3 color;
	vec3 color_components;     
};

out vec4 color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

uniform vec3 view_position;
uniform Spot_light light;

layout (std140) uniform Light_data{
	vec2 screen_size;
	vec2 resolution;
};

const int shininess = 32;

void main(){   
	vec2 frag_tex_coord = (gl_FragCoord.xy / resolution);
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).rgb);
	float distance = length(light.position - frag_position);

	vec3 view_direction = normalize(view_position - texture(g_position, frag_tex_coord).rgb);
	vec3 light_direction = normalize(light.position - frag_position); 
	vec3 reflect_direction = reflect(-light_direction, normal); 
 
	float diff = max(dot(normal, light_direction), 0.0);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	vec3 ambient = (light.color * light.color_components.x) * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);

	vec3 diffuse = (light.color * light.color_components.y) * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);

	vec3 specular = (light.color * light.color_components.z) * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
	
	float theta = degrees(acos(dot(light_direction, normalize(-light.direction)))); 
	float epsilon = (light.cut_off - light.outer_cut_off);
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	float attenuation = clamp(1.0 - distance/light.radius, 0.0, 1.0);
	attenuation *= attenuation;

	ambient	 *= intensity;
	diffuse  *= intensity;
	specular *= intensity;
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   
  
    color = vec4(ambient + diffuse + specular, 1.0);
}