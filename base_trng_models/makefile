CC=g++
CFLAGS=-c  -I".\includes"  -std=gnu++11
LDFLAGS=  -lglew32 -lSOIL -lglfw3 -lopengl32 -static-libgcc -static-libstdc++  -l:libgdi32.a -L".\gl"
SOURCES= main.cpp glresourses.cpp gl_render_target.cpp glscene.cpp gl_character.cpp gl_model.cpp gl_game_state_arena.cpp gl_game_state_dungeon.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
