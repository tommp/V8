engine_EXECUTABLE := v8
game_EXECUTABLE := engine

CXX = clang++

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
current_path := $(abspath ../$(current_dir))

program_ASSETS := $(wildcard assets/animations/*)
program_ASSETS += $(wildcard assets/models/*)
program_ASSETS += $(wildcard assets/textures/*)
program_ASSETS += $(wildcard assets/materials/*)
program_ASSETS += $(wildcard assets/meshes/*)
program_ASSETS += $(wildcard assets/config/*)

program_CXX_SRCS := $(wildcard src/*.cpp)
program_CXX_SRCS += $(wildcard projects/$(game_EXECUTABLE)/src/*.cpp)
program_OBJS := ${program_CXX_SRCS:.cpp=.o}


OPENGL_LIB := GLEW GL
SDL_LIB :=  SDL2 SDL2_image SDL2_mixer SDL2_gfx 
BULLET_LIB := BulletDynamics BulletCollision LinearMath
SOIL_LIB := SOIL
ASSIMP_LIB := assimp

program_INCLUDE_DIRS := $(current_path)/projects/$(game_EXECUTABLE)/src/headers/ $(current_path)/src/headers/ $(current_path)/src/headers/maps/ $(current_path)/src/headers/bullet3_includes $(current_path)/src/headers/glm_includes $(current_path)/src/headers/assimp_includes $(current_path)
program_LIBRARY_DIRS := /usr/local/lib $(current_path)/src/lib/static
program_RUNTIME_LIBRARY_DIRS := $(current_path)/src/lib/dynamic
program_LIBRARIES := $(OPENGL_LIB) $(SDL_LIB) $(BULLET_LIB) $(SOIL_LIB) $(ASSIMP_LIB) m 

CXXFLAGS += -Wall -g -std=c++11
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += -Wl$(foreach librarydir,$(program_RUNTIME_LIBRARY_DIRS),,-rpath=$(librarydir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))


.PHONY: all clean distclean game
all: $(engine_EXECUTABLE)

$(program_OBJS):%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CPPFLAGS)

$(engine_EXECUTABLE): $(program_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -o $(engine_EXECUTABLE) $(program_OBJS) $(LDFLAGS)

game: $(game_EXECUTABLE)

$(game_EXECUTABLE): $(program_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(LDFLAGS) -o projects/$(game_EXECUTABLE)/$(game_EXECUTABLE)/$(game_EXECUTABLE) $(program_OBJS)

clean:
	@- $(RM) $(program_EXECUTABLE)
	@- $(RM) $(program_OBJS)
	@- $(RM) $(program_ASSETS)

rmas:
	@- $(RM) $(program_ASSETS)

distclean: clean

.PHONY: all clean rmas

