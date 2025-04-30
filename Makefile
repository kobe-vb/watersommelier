# Detecteer OS-type
ifeq ($(OS),Windows_NT)
    OS_TYPE = windows
else
    OS_TYPE = unix
endif

CXX = clang++
CXXFLAGS = -g -Wall -std=c++20 -I include -I my_lib/include -I /mingw64/include

# Platform-specifieke include/lib directories
ifeq ($(OS_TYPE), windows)
    CXXFLAGS += -I /mingw64/include -DPLATFORM_WINDOWS
    LDFLAGS += -L /mingw64/lib -lgdi32 -luser32
else
    # Linux: gebruik jouw lokale raylib build
    CXXFLAGS += -I $(HOME)/raylib/include
    LDFLAGS += -L $(HOME)/raylib/lib
endif

# Release-specifieke linker flags (alleen Windows)
RELEASE_LDFLAGS = $(LDFLAGS)
ifeq ($(OS_TYPE), windows)
    RELEASE_LDFLAGS += -mwindows -static-libgcc -static-libstdc++
endif

# Source-bestanden
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
MY_LIB_SRC = $(wildcard my_lib/src/*.cpp)
MY_LIB_OBJ = $(patsubst my_lib/src/%.cpp, my_lib/obj/%.o, $(MY_LIB_SRC))

# Outputs
OUT = bin/water_woeter.exe
RELEASE_OUT = bin/game_release.exe
LIB_NAME = my_lib/lib/libmy_lib.a
RELEASE_DIR = release

# Targets

all: $(LIB_NAME) $(OUT)

$(OUT): $(OBJ) $(LIB_NAME) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(RELEASE_OUT): $(OBJ) $(LIB_NAME) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(RELEASE_LDFLAGS)

$(LIB_NAME): $(MY_LIB_OBJ) | my_lib/lib
	ar rcs $@ $^

# Mappen aanmaken indien nodig
bin obj my_lib/obj my_lib/lib:
	mkdir -p $@

# Object files compileren
obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_lib/obj/%.o: my_lib/src/%.cpp | my_lib/obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run de game
run: all
	./$(OUT)

# Opruimen
clean:
	rm -rf obj my_lib/obj bin/$(notdir $(OUT)) bin/$(notdir $(RELEASE_OUT))

fclean: clean
	rm -rf my_lib/lib bin release release.zip

re: fclean all

# Release-folder aanmaken met alle bestanden
release: $(RELEASE_OUT)
	rm -rf $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)
	cp $(RELEASE_OUT) $(RELEASE_DIR)/water_woeter.exe
	cp -r data $(RELEASE_DIR)/

ifeq ($(OS_TYPE), windows)
	cp /mingw64/bin/libraylib.dll $(RELEASE_DIR)/
	cp /mingw64/bin/glfw3.dll $(RELEASE_DIR)/
	cp /mingw64/bin/libwinpthread-1.dll $(RELEASE_DIR)/
endif
