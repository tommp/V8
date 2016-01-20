#include "./headers/shader.h"

Shader::Shader(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path){
    /* create the shaders */
	GLuint vertex_shader = create_shader(vertex_shader_path, GL_VERTEX_SHADER);
	assert(vertex_shader);

	GLuint fragment_shader = create_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
	assert(fragment_shader);

	/* Create program */
	program = glCreateProgram();

	/* Attach the shaders */
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	/* Link the program */
	GLint success;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
	{
		print_log(program);
	    assert(0);
	}

	/* Delete the linked shaders */
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

char* Shader::load_from_file(const char* filename) {
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
    const GLchar* source = load_from_file(filename);
    if (source == NULL) {
        SDLerrorLogger("Shader load_from_file(const char* filename)");
        std::cout << "Error opening " << filename << ": " << SDL_GetError() << std::endl;
        return 0;
    }

    /* Create the shader */
    GLuint res = glCreateShader(type);
    const GLchar* sources[] = {"#version 120\n", source};
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

void Shader::print_log(GLuint object) {
    GLint log_length = 0;
    if (glIsShader(object)) {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else if (glIsProgram(object)) {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else {
        errorlogger("ERROR: Argument to print_log(GLuint object), not a shader or a program!");
        std::cout << "ERROR: Argument to print_log(GLuint object), not a shader or a program!" << std::endl;
        return;
    }

    char* log = (char*)malloc(log_length);
    
    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);
    
    errorlogger("Shader compilation ERROR: ", log);
    free(log);
}