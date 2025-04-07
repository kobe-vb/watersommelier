CXX = g++
CXXFLAGS = -Wall -std=c++17 -I include -I my_lib/include -I ~/raylib/include
LDFLAGS = -L ~/raylib/lib -L my_lib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lmy_lib

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
MY_LIB_SRC = $(wildcard my_lib/src/*.cpp)
MY_LIB_OBJ = $(patsubst my_lib/src/%.cpp, my_lib/obj/%.o, $(MY_LIB_SRC))

OUT = bin/game
LIB_NAME = my_lib/lib/libmy_lib.a
RELEASE_DIR = release

all: $(LIB_NAME) $(OUT)

# Compile de game
$(OUT): $(OBJ) $(LIB_NAME) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

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
	rm -rf obj my_lib/obj bin/game

fclean: clean
	rm -rf my_lib/lib bin release

re: fclean all

# Release aanmaken zonder circulaire afhankelijkheid
release: all
	rm -rf $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)
	cp -r bin/game data $(RELEASE_DIR)/

