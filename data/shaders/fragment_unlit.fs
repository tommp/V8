
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
}; 

in vec2 frag_tex_coord;

out vec4 color;

uniform Material material;

void main()
{    
	color = texture(material.diffuse, frag_tex_coord);
}