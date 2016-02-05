program_EXECUTABLE := ../v

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
current_path := $(abspath ../$(current_dir))

program_CXX_SRCS := $(wildcard *.cpp)
program_OBJS := ${program_CXX_SRCS:.cpp=.o}

OPENGL_LIB = -lGLEW -lGL
SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx -Wl,-rpath=/usr/local/lib
program_INCLUDE_DIRS := $(current_path)/headers/ $(current_path)/headers/maps/
program_LIBRARY_DIRS :=
program_LIBRARIES := m 

LDFLAGS += $(SDL_LIB)
LDFLAGS += $(OPENGL_LIB)
CXXFLAGS += -Wall -g -std=c++11
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
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

