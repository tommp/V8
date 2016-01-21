#include "./headers/utility.h"

void waitForEvent(){
	SDL_Event event;
	bool done = false;
	while((!done) && (SDL_WaitEvent(&event))) {
        switch(event.type) {
    
            case SDL_KEYDOWN:
                done = true;
                break;


            case SDL_QUIT:
                done = true;
                break;
                
            default:
                break;
        } 
            
    }
}

void load_binary_image(std::string name, unsigned char* image, GLint* width, GLint* height){
    std::ifstream registerf (IMAGE_DATA_INDEX_PATH, std::ios::binary);
    std::ifstream contentf (IMAGE_DATA_PATH, std::ios::binary);

    GLuint  id = ENGINE_IMAGES[name];
    GLuint datapos = -1;
    GLuint read_id;

    while(!registerf.eof()){
        registerf.read(reinterpret_cast<char *>(&read_id), sizeof(GLuint));
        if (read_id == id) {
            registerf.read(reinterpret_cast<char *>(&datapos), sizeof(GLuint));
            registerf.read(reinterpret_cast<char *>(width), sizeof(GLuint));
            registerf.read(reinterpret_cast<char *>(height), sizeof(GLuint));
            break;
        }
        else{
            registerf.seekg(sizeof(GLuint)*3, std::ios_base::cur);
        }
    }

    if (datapos >= 0){
        contentf.seekg(datapos);
        contentf.read(reinterpret_cast<char *>(&format), sizeof(GLint));
        contentf.read(reinterpret_cast<char *>(image), sizeof(unsigned char)*(*width)*(*height));
    }
    else{
        std::cout << "FILEFAIL\n" << std::endl;
    }
    registerf.close();
    contentf.close();
}