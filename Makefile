# Detecteer besturingssysteem
ifeq ($(OS),Windows_NT)
    OS_TYPE = windows
else
    OS_TYPE = unix
endif

CXX = clang++
CXXFLAGS = -g -Wall -std=c++17 -I include -I my_lib/include -I /mingw64/include

# Standaard LDFLAGS (geen Linux-specifieke libraries hier)
LDFLAGS = -L /mingw64/lib -L my_lib/lib -lgdi32 -luser32 -lmy_lib -lraylib

# Voor Windows, pas de flags aan
ifeq ($(OS_TYPE), windows)
    CXXFLAGS += -DPLATFORM_WINDOWS
    LDFLAGS += -lgdi32 -luser32 -lmy_lib
endif

# Windows Release-specifieke flags
RELEASE_LDFLAGS = $(LDFLAGS)
ifeq ($(OS_TYPE), windows)
    RELEASE_LDFLAGS += -mwindows -static-libgcc -static-libstdc++
endif

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
MY_LIB_SRC = $(wildcard my_lib/src/*.cpp)
MY_LIB_OBJ = $(patsubst my_lib/src/%.cpp, my_lib/obj/%.o, $(MY_LIB_SRC))

OUT = bin/water_woeter.exe
RELEASE_OUT = bin/game_release.exe
LIB_NAME = my_lib/lib/libmy_lib.a
RELEASE_DIR = release

all: $(LIB_NAME) $(OUT)

# Compile de game (development versie met console)
$(OUT): $(OBJ) $(LIB_NAME) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

# Compile release versie (zonder console)
$(RELEASE_OUT): $(OBJ) $(LIB_NAME) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(RELEASE_LDFLAGS)

# Compile my_lib als een statische library
$(LIB_NAME): $(MY_LIB_OBJ) | my_lib/lib
	ar rcs $@ $^

# Zorg ervoor dat alle mappen bestaan voordat er bestanden in worden gezet
bin obj my_lib/obj my_lib/lib:
	mkdir -p $@

# Compile individuele object files
obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_lib/obj/%.o: my_lib/src/%.cpp | my_lib/obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Extra commando's
run: all
	./$(OUT)

clean:
	rm -rf obj my_lib/obj bin/game.exe bin/game_release.exe

fclean: clean
	rm -rf my_lib/lib bin release release.zip 

re: fclean all

release: $(RELEASE_OUT)
	rm -rf $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)
	cp -r bin/game_release.exe $(RELEASE_DIR)/water_woeter.exe
	cp -r data $(RELEASE_DIR)/
	cp /mingw64/bin/libraylib.dll $(RELEASE_DIR)/
	cp /mingw64/bin/glfw3.dll $(RELEASE_DIR)/
	cp /mingw64/bin/libwinpthread-1.dll $(RELEASE_DIR)/

# cd /c/Users/32477/OneDrive\ -\ Thomas\ More/Documenten/1school/tibs
# min