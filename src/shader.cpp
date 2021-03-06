#include "shader.h"

std::string Shader_vars::opengl_version = "#version 330 core\n";

Shader::Shader(){
    program = 0;
}

bool Shader::use(){
    glUseProgram(program);
    if(check_ogl_error()){
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use shader: " << name << std::endl;
        errorlogger("ERROR: Failed to use shader:", name.c_str());
        return false;
    }

    return true;
}

char* Shader::read_data_from_file(const char* filename) {
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
    if (rw == NULL){
        return NULL;
    } 
    
    Sint64 res_size = SDL_RWsize(rw);
    char* res = (char*)malloc(res_size + 1);

    Sint64 nb_read_total = 0;
    Sint64 nb_read = 1;
    char* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return NULL;
    }
    
    res[nb_read_total] = '\0';
    return res;
}

GLuint Shader::create_shader(const char* filename, GLenum type) {

    /* Load shader code from file */
    const GLchar* source = read_data_from_file(filename);
    if (source == NULL) {
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Could not open shader source file with name: " << filename << ": " << SDL_GetError() << std::endl;
        SDLerrorLogger("Shader load_from_file(const char* filename)");
        return 0;
    }

    /* Create the shader */
    GLuint res = glCreateShader(type);
    const GLchar* sources[] = {Shader_vars::opengl_version.c_str(), source};
    glShaderSource(res, 2, sources, NULL);

    /* Delete the buffered source code */
    free((void*)source);

    /* Compile the shader */
    GLint compile_ok = GL_FALSE;
    glCompileShader(res);

    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        print_log(res);
        glDeleteShader(res);
        return 0;
    }
    
    return res;
}

bool Shader::load_from_file(const std::string& name){
    if (WORLD_SHADERS.find(name) == WORLD_SHADERS.end()) {
        std::cout << "ERROR: Shader not found!: " << name << std::endl;
        errorlogger("ERROR: Shader not found!: ", name.c_str());
        return false;
    }

    GLboolean geom_shader_found = false;
    if (GEOM_SHADERS.find(name) != GEOM_SHADERS.end()) {
        geom_shader_found = true;
    }

    GLuint geometry_shader = 0;
    if (geom_shader_found){
        std::string geometry_path = Utility_vars::folder_path + SHADER_PATH + GEOM_SHADERS.find(name)->second;
        
        const GLchar* geometry_shader_path = geometry_path.c_str();
        
        geometry_shader = create_shader(geometry_shader_path, GL_GEOMETRY_SHADER);
        if(!geometry_shader) {
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to create geometry shader: " << geometry_shader_path << std::endl;
            errorlogger("ERROR: Failed to create geometry shader: ", geometry_shader_path);
            return false;
        }
    }

    this->name = name;

    std::string vertex_path = Utility_vars::folder_path + SHADER_PATH + WORLD_SHADERS.find(name)->second.first;
    std::string fragment_path = Utility_vars::folder_path + SHADER_PATH + WORLD_SHADERS.find(name)->second.second;
    
    const GLchar* vertex_shader_path = vertex_path.c_str();
    const GLchar* fragment_shader_path = fragment_path.c_str();

    /* create the shaders */
    GLuint vertex_shader = create_shader(vertex_shader_path, GL_VERTEX_SHADER);
    if(!vertex_shader) {
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to create vertex shader: " << vertex_shader_path << std::endl;
        errorlogger("ERROR: Failed to create vertex shader: ", vertex_shader_path);
        return false;
    }

    GLuint fragment_shader = create_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
    if(!fragment_shader) {
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to create fragment shader: " << fragment_shader_path << std::endl;
        errorlogger("ERROR: Failed to create fragment_shader: ", fragment_shader_path);
        return false;
    }

    /* Create program */
    program = glCreateProgram();

    /* Attach the shaders */
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    if (geometry_shader) {
        glAttachShader(program, geometry_shader);
    }
    else{
        SDL_Log("No geometry shader for: %s", name.c_str());
    }

    /* Link the program */
    GLint success;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        print_log(program);
        return false;
    }

    /* Delete the linked shaders */
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    if (geometry_shader){
        glDeleteShader(geometry_shader);
    }

    if(check_ogl_error()){
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to delete bound shaders in shader: " << name << std::endl;
        errorlogger("ERROR: Failed to delete bound shaders in shader load_from_file()!");
        return false;
    }

    if (!bind_uniform_buffers(program, name)){
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind uniform buffers in shader load_from_file()" << std::endl;
        errorlogger("ERROR: Failed to bind uniform buffers in shader: ", name.c_str());
        return false;
    }

    return true;
}

bool Shader::bind_uniform_buffers(GLuint program, const std::string& name)const{
    GLuint uniform_block_index_matrices = glGetUniformBlockIndex(program, "Matrices");
    if (uniform_block_index_matrices == GL_INVALID_INDEX) {
        SDL_Log("No matrix data uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_matrices, 1);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind matrix uniform buffer in shader: " << name << std::endl;
            errorlogger("ERROR: Failed to bind matrix uniform buffer in shader: ", name.c_str());
            return false;
        }
    }

    GLuint uniform_block_index_res_data = glGetUniformBlockIndex(program, "Resolution_data");
    if (uniform_block_index_res_data == GL_INVALID_INDEX) {
        SDL_Log("No resolution data uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_res_data, 2);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind resolution data uniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind resolution data uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    GLuint uniform_block_index_plane_data = glGetUniformBlockIndex(program, "Plane_data");
    if (uniform_block_index_plane_data == GL_INVALID_INDEX) {
        SDL_Log("No plane data uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_plane_data, 3);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind plane datauniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind plane data uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    GLuint uniform_block_index_settings = glGetUniformBlockIndex(program, "Settings");
    if (uniform_block_index_settings == GL_INVALID_INDEX) {
        SDL_Log("No settings uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_settings, 4);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind settings data uniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind settings uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    GLuint uniform_block_index_cam_data = glGetUniformBlockIndex(program, "Camera_data");
    if (uniform_block_index_cam_data == GL_INVALID_INDEX) {
        SDL_Log("No camera data uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_cam_data, 5);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind camera data uniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind camera uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    GLuint uniform_block_index_dir_lights = glGetUniformBlockIndex(program, "Dir_lights");
    if (uniform_block_index_dir_lights == GL_INVALID_INDEX) {
        SDL_Log("No directional light uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_dir_lights, 6);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind directional light uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    GLuint uniform_block_index_point_lights = glGetUniformBlockIndex(program, "Point_lights");
    if (uniform_block_index_point_lights == GL_INVALID_INDEX) {
        SDL_Log("No point light uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_point_lights, 7);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind point light uniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind point light uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    GLuint uniform_block_index_spot_lights = glGetUniformBlockIndex(program, "Spot_lights");
    if (uniform_block_index_spot_lights == GL_INVALID_INDEX) {
        SDL_Log("No spot light uniform buffer for shader: %s, assuming expected behaviour!", name.c_str());
    }
    else{
        glUniformBlockBinding(program, uniform_block_index_spot_lights, 8);
        if(check_ogl_error()){
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind spot light uniform buffer in shader: "<< name  << std::endl;
            errorlogger("ERROR: Failed to bind spot light uniform buffer in shader: ", name.c_str());
            return false;
        }        
    }

    return true;
}

void Shader::print_log(GLuint object) {
    GLint log_length = 0;
    if (glIsShader(object)) {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else if (glIsProgram(object)) {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else {
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Argument to print_log(GLuint object), not a shader or a program!" << std::endl;
        errorlogger("ERROR: Argument to print_log(GLuint object), not a shader or a program!");
        return;
    }

    char* logger= (char*)malloc(log_length);
    
    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, logger);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, logger);
    
    std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Shader compilation failed: " << logger << std::endl;
    errorlogger("ERROR: Shader compilation failed: ", logger);
    free(logger);
}

GLuint Shader::load_uniform_location(const std::string& uniform){
    if (uniform_locations.find(uniform) != uniform_locations.end()){
        return uniform_locations[uniform];
    }
    else{
        GLuint new_uniform_location = glGetUniformLocation(program, uniform.c_str());
        if(check_ogl_error()) {
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load uniform loacation for uniform: " << uniform << std::endl;
            errorlogger("ERROR: Failed to load uniform loacation for uniform: ", uniform.c_str());
            return -1;
        }
        uniform_locations[uniform] = new_uniform_location;
        return new_uniform_location;
    }
}

GLuint Shader::load_uniform_location(const std::string& uniform, GLuint uniform_index){
    std::string uniform_name = static_cast<std::ostringstream*>( &(std::ostringstream() << uniform << "[" << uniform_index << "]") )->str();
    if (uniform_locations.find(uniform_name) != uniform_locations.end()){
        return uniform_locations[uniform_name];
    }
    else{
        GLuint new_uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(check_ogl_error()) {
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to get uniform location for uniform: " << uniform_name << std::endl;
            errorlogger("ERROR: Failed to get uniform location for uniform: ", uniform_name.c_str());
            return -1;
        }
        uniform_locations[uniform_name] = new_uniform_location;
        return new_uniform_location;
    }
}

GLuint Shader::load_uniform_location(const std::string& uniform, GLuint uniform_index, const std::string& uniform_member){
    std::string uniform_name = static_cast<std::ostringstream*>( &(std::ostringstream() << uniform << "[" << uniform_index << "]" << "." << uniform_member) )->str();
    if (uniform_locations.find(uniform_name) != uniform_locations.end()){
        return uniform_locations[uniform_name];
    }
    else{
        GLuint new_uniform_location = glGetUniformLocation(program, uniform_name.c_str());
        if(check_ogl_error()) {
            std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to get uniform location for uniform: " << uniform_name << std::endl;
            errorlogger("ERROR: Failed to get uniform location for uniform: ", uniform_name.c_str());
            return -1;
        }
        uniform_locations[uniform_name] = new_uniform_location;
        return new_uniform_location;
    }
}