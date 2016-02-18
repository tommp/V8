program_EXECUTABLE := ../v

CXX = clang++

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
current_path := $(abspath ../$(current_dir))

program_CXX_SRCS := $(wildcard *.cpp)
program_OBJS := ${program_CXX_SRCS:.cpp=.o}

OPENGL_LIB := GLEW GL
SDL_LIB :=  SDL2 SDL2_image SDL2_mixer SDL2_gfx 
BULLET_LIB := BulletDynamics BulletCollision LinearMath

program_INCLUDE_DIRS := $(current_path)/headers/ $(current_path)/headers/maps/ 
program_LIBRARY_DIRS := /usr/local/lib lib/static/ 
program_LIBRARIES := $(OPENGL_LIB) $(SDL_LIB) $(BULLET_LIB) m 

CXXFLAGS += -Wall -g -std=c++11
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += -Wl,-rpath=/usr/local/lib
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))


.PHONY: all clean distclean
all: $(program_EXECUTABLE)

$(program_OBJS):%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CPPFLAGS)

$(program_EXECUTABLE): $(program_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(LDFLAGS) -o $(program_EXECUTABLE) $(program_OBJS)

clean:
	@- $(RM) $(program_EXECUTABLE)
	@- $(RM) $(program_OBJS)

distclean: clean

.PHONY: all clean

