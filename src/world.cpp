#include "world.h"

World::World(Resource_manager& init_manager){
	manager = &init_manager;
	light_shader = init_manager.load_shader("light_shader");
	current_level = std::make_shared<Level>(Level(4000, 4000, 200));
	Character_ptr player = std::make_shared<Player>(init_manager);
	add_player(player);

	
	for (int i = 0; i < 200; ++i) {
		Character_ptr cube = std::make_shared<Cube>(init_manager);
		insert_character(cube);
	}

	GLfloat quad_vertices[] = {
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	GLuint quad_VBO;
	glGenVertexArrays(1, &quad_VAO);
	glGenBuffers(1, &quad_VBO);
	glBindVertexArray(quad_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

World::~World() {
}

bool World::check_if_colliding(const Character_ptr& a, const Character_ptr& b)const{

	if(!a || !b){
		return false;
	}

	if( ( a->get_y() + a->get_height() ) <= b->get_y() ){
		return false;
	}

	if( a->get_y() >= ( b->get_y() + b->get_height() ) ){
		return false;
	}

	if( ( a->get_x() + a->get_width() ) <= b->get_x() ){
		return false;
	}

	if( a->get_x() >= ( b->get_x() + b->get_width() ) ){
		return false;
	}

	if( ( a->get_z() + a->get_depth() ) <= b->get_z() ){
		return false;
	}

	if( a->get_z() >= ( b->get_z() + b->get_depth() ) ){
		return false;
	}

	/* If none of the sides from A are outside B */
	return true;
}

bool World::check_if_offscreen(const Character_ptr& a)const{
	return false;
}

void World::update_positions(GLfloat timedelta){
	
	for (auto it = players.begin(); it != players.end(); ++it) {
		(*it)->update_position(timedelta);
	}
	for (auto it = characters.begin(); it != characters.end(); ++it) {
		(*it)->update_position(timedelta);
	}

	if(!players.empty()){
		current_level->center_camera(players.front());
	}
	current_level->get_camera_pointer()->update_view_matrix();
	current_level->get_camera_pointer()->upload_view_matrix(manager->get_uniform_buffer("matrices"));
}


void World::detect_all_collisions() {
	if (!players.empty()){
		detect_collisions(players);
		detect_collisions(players, characters);
	}

	if (!characters.empty()){
		detect_collisions(characters);
	}
}

void World::detect_collisions(const std::list<Character_ptr>& a){
	if (!a.empty()) {
		for (auto it_a = a.begin(); it_a != a.end(); ++it_a) {
			for (auto it_a_2 = it_a; it_a_2 != a.end(); ++it_a_2) {
				if(check_if_colliding( *(it_a), *(it_a_2) )){
					if (it_a == it_a_2) {
						continue;
					}
					else{
						contacts.emplace_front( *(it_a), *(it_a_2) );
					}
				}
				else{
					continue;
				}
			}
		}
	}
}

void World::detect_collisions(const std::list<Character_ptr>& a, const std::list<Character_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); ++it_a) {
		for (auto it_b = b.begin(); it_b != b.end(); ++it_b) {
			if(check_if_colliding( *(it_a), *(it_b) )){
				contacts.emplace_front( *(it_a), *(it_b) );
			}
			else{
				continue;
			}
		}
	}
}

void World::update_groups(){
	auto first_it_dormant = dormant_characters.begin();
	auto before_it_dormant = dormant_characters.before_begin();

	for (auto it = characters.begin(); it != characters.end(); ++it){
		if( !check_if_offscreen(*it)){
			add_dormant_character(*it);
			it = characters.erase(it);
		}
	}

	for (auto it = first_it_dormant; it != dormant_characters.end(); ++it){
		if(check_if_offscreen(*it)){
			insert_character(*it);
			dormant_characters.erase_after(before_it_dormant);
			it = before_it_dormant;
		}
		else{
			++before_it_dormant;
		}
	}
}

void World::render_quad()const{
	glBindVertexArray(quad_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void World::render_geometry()const{
	manager->use_g_buffer(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto player : players) {
		player->render_frame();
	}

	for (auto character : characters) {
		character->render_frame();
	}
	manager->use_g_buffer(false);

	if(check_ogl_error()){
		errorlogger("ERROR: Failed to render geometry!");
		std::cout << "ERROR: Failed torender geometry! File: " << __FILE__ << ", line: " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
}

void World::render_lights()const{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	light_shader->use();
	manager->bind_g_data(light_shader);

	for (auto light : lights) {
		light->render_light();
	}
	render_quad();

	if(check_ogl_error()){
		errorlogger("ERROR: Failed to render lights!");
		std::cout << "ERROR: Failed to render lights! File: " << __FILE__ << ", line: " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
}


void World::render_world()const{
	render_geometry();
	render_lights();
	manager->present_display();
}

bool World::insert_character(const Character_ptr& character){
	if (character){
		characters.push_back(character);
		return true;
	}
	else{
		errorlogger("ERROR: Cannot add null Character in world");
		std::cout << "ERROR: Cannot add null Character in world" << std::endl;
		return false;
	}
}

bool World::add_dormant_character(const Character_ptr& character){
	if(character){
		dormant_characters.push_front(character);
		return true;
	}
	else{
		errorlogger("ERROR: Cannot add null character in world");
		std::cout << "ERROR: Cannot add null character in world" << std::endl;
		return false;
	}
}

bool World::add_character(const Character_ptr& character){
	if(character) {
		if(check_if_offscreen(character)) {
			return insert_character(character);
		}
		else{
			return add_dormant_character(character);
		}
	}
	else{
		errorlogger("ERROR: Cannot add null character in world");
		std::cout << "ERROR: Cannot add null character in world" << std::endl;
		return false;
	}
}

bool World::insert_player(const Character_ptr& player){
	if (player){
		players.push_back(player);	
		return true;
	}
	else{
		errorlogger("ERROR: Cannot add null player in World");
		std::cout << "ERROR: Cannot add null player in World" << std::endl;
		return false;
	}
}


bool World::add_player(const Character_ptr& player){
	return insert_player(player);
}

void World::resolve_collisions(){
	contacts.clear();
}