CC=g++
ENGINE=..\gl_engine
CFLAGS=-c  -I".\includes" -I".\glm" -I"$(ENGINE)"   -std=gnu++11
ifeq ($(OS),Windows_NT)
	DELOPERATOR= del /q
	LDFLAGS=  -lglew32 -lSOIL -lglfw3 -lopengl32 -static-libgcc -static-libstdc++  -l:libgdi32.a -L".\gl"
else
	DELOPERATOR= rm
	LDFLAGS=  -lGLEW -lSOIL -lGL `pkg-config --cflags glfw3` `pkg-config --static --libs glfw3`
endif
ENGINE_SOURCES= $(ENGINE)\glresourses.cpp $(ENGINE)\glscene.cpp $(ENGINE)\collision.cpp $(ENGINE)\gl_jal_mesh_atlas.cpp $(ENGINE)\gl_texture_atlas.cpp
SOURCES= $(ENGINE_SOURCES) main.cpp loader.cpp map_event_general.cpp map_event_valhalla.cpp map_event_hero_strikes.cpp  gl_resources_manager.cpp gl_render_target.cpp gl_character.cpp gl_model.cpp gl_game_state_arena.cpp gl_game_state_dungeon.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game

all: build

build:	$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

rebuild: clean build

clean:
	$(DELOPERATOR) $(OBJECTS)