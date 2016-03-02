engine_EXECUTABLE := v8
game_EXECUTABLE := engine

CXX = clang++

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
current_path := $(abspath ../$(current_dir))

program_CXX_SRCS := $(wildcard src/*.cpp)
program_CXX_SRCS += $(wildcard projects/$(game_EXECUTABLE)/src/*.cpp)
program_OBJS := ${program_CXX_SRCS:.cpp=.o}


OPENGL_LIB := GLEW GL
SDL_LIB :=  SDL2 SDL2_image SDL2_mixer SDL2_gfx 
BULLET_LIB := BulletDynamics BulletCollision LinearMath

program_INCLUDE_DIRS := $(current_path)/projects/$(game_EXECUTABLE)/src/headers/ $(current_path)/src/headers/ $(current_path)/src/headers/maps/ $(current_path)/src/headers/bullet3_includes $(current_path)/src/headers/glm_includes
program_LIBRARY_DIRS := /usr/local/lib 
program_RUNTIME_LIBRARY_DIRS := $(current_path)/src/lib
program_LIBRARIES := $(OPENGL_LIB) $(SDL_LIB) $(BULLET_LIB) m 

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
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(LDFLAGS) -o $(engine_EXECUTABLE) $(program_OBJS)

game: $(game_EXECUTABLE)

$(game_EXECUTABLE): $(program_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(LDFLAGS) -o projects/$(game_EXECUTABLE)/$(game_EXECUTABLE)/$(game_EXECUTABLE) $(program_OBJS)

clean:
	@- $(RM) $(program_EXECUTABLE)
	@- $(RM) $(program_OBJS)

distclean: clean

.PHONY: all clean

