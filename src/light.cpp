#include "light.h"

Directional_light::Directional_light(){
	type = DIRECTIONAL;
	direction = {1.0f, -1.0f, 1.0f};

	ambient = {0.01f, 0.01f, 0.01f};
	diffuse = {0.01f, 0.01f, 0.01f};
	specular = {0.01f, 0.01f, 0.01f};
}

Point_light::Point_light(){
	type = POINT;
	position = {0.0f, 100.0f, 0.0f};
	position.x = rand() % 4001;
	position.z = rand() % 4001;

	ambient = {0.0f, 0.0f, 0.0f};
	diffuse = {1.0f, 1.0f, 1.0f};
	specular = {1.0f, 1.0f, 1.0f};

	constant = 1.0f;
	linear = 0.0014f;
	quadratic = 0.000007f;
}

Spot_light::Spot_light(){
	type = SPOT;
	direction = {0.0f, -1.0f, 0.0f};
	position = {0.0f, 200.0f, 0.0f};
	position.x = rand() % 4001;
	position.z = rand() % 4001;

	ambient = {0.2f, 0.2f, 0.2f};
	diffuse = {10.0f, 10.0f, 10.0f};
	specular = {1.0f, 1.0f, 1.0f};

	constant = 1.0f;
	linear = 0.0014f;
	quadratic = 0.000007f;
}

void Directional_light::render_light(const Renderer& renderer, GLuint light_number)const{
    glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("directional_lights[" + std::to_string(light_number) + "].direction").c_str()), 1, (float*)&(direction));
 	
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("directional_lights[" + std::to_string(light_number) + "].ambient").c_str()), 1, (float*)&(ambient));
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("directional_lights[" + std::to_string(light_number) + "].diffuse").c_str()), 1, (float*)&(diffuse));
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("directional_lights[" + std::to_string(light_number) + "].specular").c_str()), 1, (float*)&(specular));
 	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind directional light uniforms!");
	}
 }

void Point_light::render_light(const Renderer& renderer, GLuint light_number)const{
    glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].position").c_str()), 1, (float*)&(position));
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].constant").c_str()), constant);
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].linear").c_str()), linear);
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].quadratic").c_str()), quadratic);

    glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].ambient").c_str()), 1, (float*)&(ambient));
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].diffuse").c_str()), 1, (float*)&(diffuse));
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("point_lights[" + std::to_string(light_number) + "].specular").c_str()), 1, (float*)&(specular));
 	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind point light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind point light uniforms!");
	}
}

void Spot_light::render_light(const Renderer& renderer, GLuint light_number)const{
    glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].position").c_str()), 1, (float*)&(position));
    glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("lights[" + std::to_string(light_number) + "].direction").c_str()), 1, (float*)&(direction));
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].cut_off").c_str()), cut_off);
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].outer_cut_off").c_str()), outer_cut_off);
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].constant").c_str()), constant);
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].linear").c_str()), linear);
    glUniform1f(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].quadratic").c_str()), quadratic);

    glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].ambient").c_str()), 1, (float*)&(ambient));
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].diffuse").c_str()), 1, (float*)&(diffuse));
 	glUniform3fv(glGetUniformLocation(renderer.get_light_shader(), ("spot_lights[" + std::to_string(light_number) + "].specular").c_str()), 1, (float*)&(specular));
 	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind spot light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind spot light uniforms!");
	}
}