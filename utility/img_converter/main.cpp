#include "headers/main.h"

int main(int argc, char** argv) {
	std::remove(REGISTERFILE);
	std::remove(CONTENTFILE);

	std::vector<std::string> filenames = glob(IMAGES_DIR);
	std::ofstream image_header (ID_FILE);
	image_header << "#ifndef IMAGE_MAP_H\n" << "#define IMAGE_MAP_H\n\n";
	image_header << "#include <unordered_map>\n#include <string>\n\n";
	image_header << "const std::unordered_map<std::string, unsigned int> ENGINE_IMAGES = {\n";
	Image image;
	unsigned int id = 0;
	for (auto it : filenames) {
		/* Convert to binary and store */
		image.load_image(it.c_str(), id, SOIL_LOAD_RGBA);
		image.store_as_binary(REGISTERFILE, CONTENTFILE);

		/* Get pure filename without type postfix */
		it.replace(it.begin(), it.begin() + 9, "");
		it.replace(it.end() - 4, it.end(), "");
		image_header << "	{" << "\"" << it.c_str() << "\"" << ", " << id << "},\n";
		id ++;
	}

	image_header << "};\n#endif\n";
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