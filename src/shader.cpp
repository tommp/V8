#include "shader.h"

Shader::Shader(){
    program = 0;
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
    const GLchar* sources[] = {OPENGL_VERSION, source};
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

    const GLchar* vertex_shader_path = WORLD_SHADERS.find(name)->second.first.c_str();
    const GLchar* fragment_shader_path = WORLD_SHADERS.find(name)->second.second.c_str();

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

    if(check_ogl_error()){
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to delete bound shaders in shader load_from_file()" << std::endl;
        errorlogger("ERROR: Failed to delete bound shaders in shader load_from_file()!");
        exit(EXIT_FAILURE);
    }

    /* Bind the uniform buffer object for view and projection matrices */
    GLuint uniform_block_index_matrices = glGetUniformBlockIndex(program, "Matrices");
    glUniformBlockBinding(program, uniform_block_index_matrices, 1);
    if(check_ogl_error()){
        std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind matrix uniform buffer in shader load_from_file()" << std::endl;
        errorlogger("ERROR: Failed to bind matrix uniform buffer in shader load_from_file()!");
        exit(EXIT_FAILURE);
    }

    return true;
}

void Shader::use_shader_and_set_matrix4(const char* uniform, const glm::mat4& matrix){
    use();
    GLint matrix_location = glGetUniformLocation(program, uniform);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::use_shader_and_set_vector4f(const char* uniform, const glm::vec4& vector){
    use();
    GLint vector_location = glGetUniformLocation(program, uniform);
    glUniform4f(vector_location, vector[0], vector[1], vector[2], vector[3]);
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