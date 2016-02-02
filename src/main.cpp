#include "main.h"

int main(int argc, char** argv){

	/* INITIAL SETUP */
	/* ====================================== */

	/*Initializes SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		SDLerrorLogger("SDL initialization");
		std::cout<<"Failed to initialize SDL, see errorlog for details."<<std::endl;
		exit(EXIT_FAILURE);
	}

	/* Random seed to time */
    srand (time(NULL));
    /* ====================================== */

	/* MAIN VARS */
	/* ====================================== */
	Display display;

	Resource_manager resource_manager;

	Button_mappings button_mappings;

	State_handler state_handler(resource_manager);

	World world(resource_manager);

    Timer cap_timer;
    Timer move_timer;
    display.update_projection_matrix();
    display.upload_projection_matrix(resource_manager.get_uniform_buffer("matrices"));
    /* ====================================== */

    /* Testcode */
    /*
    GLfloat vertices[] = {
	        -0.5, -0.5, -0.5,
	         0.5, -0.5, -0.5,
	         0.5,  0.5, -0.5,
	         0.5,  0.5, -0.5,
	        -0.5,  0.5, -0.5,
	        -0.5, -0.5, -0.5,
	    };

	Shader_ptr testing = resource_manager.load_shader("base_shader");
	testing->use();

    GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
  
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), 
				 vertices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 
	if(check_ogl_error()){
		errorlogger("ERROR: Failed to bind!");
		std::cout << "ERROR: Failed to bind!" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create transformations
    glm::mat4 model;
    model = glm::translate(model, {0.0, 0.0, 1.0});
    model = glm::rotate(model, 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
    
    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(testing->get_program(), "model");
    //GLint viewLoc = glGetUniformLocation(testing->get_program(), "view");
    //GLint projLoc = glGetUniformLocation(testing->get_program(), "projection");
    // Pass the matrices to the shader
     testing->use();
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/
    /* ======================0 */

	/* Main loop */
	/*=======================================================*/
    while(state_handler.game_is_running()){

#if DISABLE_VSYNC
    	//Start cap timer
        cap_timer.restart();
#endif

	    /* Handle events in the queue */
	    state_handler.handle_events();

	    /* Update the position of all world objects */
	    float timedelta = move_timer.get_ticks() / 1000.f;
	    world.update_positions(timedelta);
	    move_timer.restart();

	    /* Detect collisions */
	    //world.detect_all_collisions();
	    
	    /* Resolve all collisions */
	    //cradlands.resolve_collisions();

	    /* Render to screen */
	    display.clear();
	    //glBindVertexArray(VAO);
	    world.render_world();
	    //glDrawArrays(GL_TRIANGLES, 0, 6);
	    //glBindVertexArray(0);
	    display.present();
	    if(check_ogl_error()){
			errorlogger("ERROR: Failed to render mesh!");
			std::cout << "ERROR: Failed to render mesh!" << std::endl;
			exit(EXIT_FAILURE);
		}

#if DISABLE_VSYNC
        /* If frame finished early */
        int frame_ticks = cap_timer.get_ticks();
        if( frame_ticks < SCREEN_TICKS_PER_FRAME )
        {
            /* Wait remaining time */
            SDL_Delay( SCREEN_TICKS_PER_FRAME - frame_ticks );
        }
#endif
	}
	/*=======================================================*/

	/* Store display setings */
	display.save_settings();
    /*Always be sure to clean up*/
    SDL_Quit();
    return 0;
}