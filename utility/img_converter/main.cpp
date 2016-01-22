#include "headers/main.h"

int main(int argc, char** argv) {
	std::remove(CONTENTFILE);

	std::vector<std::string> filenames = glob(IMAGES_DIR);
	std::ofstream image_header (ID_FILE);
	image_header << "#ifndef IMAGE_MAP_H\n" << "#define IMAGE_MAP_H\n\n";
	image_header << "#include <unordered_map>\n#include <string>\n\n";
	image_header << "const std::unordered_map<std::string, unsigned int> ENGINE_IMAGES = {\n";
	unsigned int id = 0;
	for (auto it : filenames) {
		GLint width, height;
		unsigned char* image = SOIL_load_image(it.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		
		/* Convert to binary and store */
		store_as_binary(&id, image, width, height, SOIL_LOAD_RGBA, CONTENTFILE);

		/* Get pure filename without type postfix */
		it.replace(it.begin(), it.begin() + 9, "");
		it.replace(it.end() - 4, it.end(), "");
		image_header << "	{" << "\"" << it.c_str() << "\"" << ", " << id << "},\n";
		free(image);
	}
	image_header << "};\n\n#endif\n";
	return 0;
}

std::vector<std::string> glob(const std::string& pat){
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    std::vector<std::string> ret;
    for(unsigned int i=0; i<glob_result.gl_pathc; i++){
        ret.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

void store_as_binary(GLuint* id, unsigned char* image, GLint width, GLint height, GLint format, char* datafile){
	std::ofstream contentf (datafile, std::ios::binary | std::ios::ate | std::ios::app);

	GLuint currentpos = contentf.tellp();
	*id = currentpos;

	contentf.write(reinterpret_cast<const char *>(&width), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&height), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&format), sizeof(GLint));
	contentf.write(reinterpret_cast<const char *>(image), sizeof(unsigned char)*width*height);

	contentf.close();
}