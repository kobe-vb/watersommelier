# Detecteer OS-type
ifeq ($(OS),Windows_NT)
    OS_TYPE = windows
else
    OS_TYPE = unix
endif

# TODO refacotr, maar vooral mijn my_lib idepandedn maken

# Compilers
CXX = clang++

INCLUDEDIRS := $(shell find include my_lib/include -type d)
INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS))

# Debug flags
CXXFLAGS = -g -Wall -Wextra -Werror -DDEBUG=1 -std=c++20 $(INCLUDES)

RELEASE_CXXFLAGS = -std=c++20 $(INCLUDES) \
    -O3 \
    -march=native \
    -mtune=native \
    -ffast-math \
    -funroll-loops \
    -finline-functions \
    -fomit-frame-pointer \
    -DNDEBUG \
    -Wall -Wextra

RELEASE_CXXFLAGS += -pthread

# Platform-specifieke includes en libraries
ifeq ($(OS_TYPE), windows)
    CXXFLAGS += -I /mingw64/include -DPLATFORM_WINDOWS
    LDFLAGS = -L /mingw64/lib -lgdi32 -luser32 -lraylib -lws2_32

    RELEASE_CXXFLAGS += -I /mingw64/include -DPLATFORM_WINDOWS
    # Voeg flags toe om duplicate section warnings te verminderen
    RELEASE_LDFLAGS = $(LDFLAGS) -mwindows -static-libstdc++ -Wl,--allow-multiple-definition
#     RELEASE_LDFLAGS = $(LDFLAGS) -static-libstdc++ -Wl,--allow-multiple-definition
else
    CXXFLAGS += -I $(HOME)/raylib/include
    LDFLAGS = -L $(HOME)/raylib/lib -lraylib -lm -ldl -lpthread -lX11 -Lmy_lib/lib -lmy_lib
    RELEASE_CXXFLAGS += -I $(HOME)/raylib/include
    RELEASE_LDFLAGS = $(LDFLAGS)
endif

# Bestandsstructuur
SRC := $(shell find src -name '*.cpp')
MY_LIB_SRC := $(wildcard my_lib/src/*.cpp)

OBJ := $(patsubst src/%.cpp,obj/%.o,$(SRC))
MY_LIB_OBJ := $(patsubst my_lib/src/%.cpp,my_lib/obj/%.o,$(MY_LIB_SRC))

RELEASE_OBJ := $(patsubst src/%.cpp,release_obj/%.o,$(SRC))
RELEASE_MY_LIB_OBJ := $(patsubst my_lib/src/%.cpp,release_obj/my_lib_%.o,$(MY_LIB_SRC))

# Outputs
OUT = bin/pompion.exe
RELEASE_OUT = bin/game_release.exe
LIB_NAME = my_lib/lib/libmy_lib.a
RELEASE_DIR = release
RES_OBJ = resources/icon.o

# Targets
all: $(LIB_NAME) $(OUT)

$(OUT): $(OBJ) $(MY_LIB_OBJ) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

resources/icon.o: resources/icon.rc resources/icon.ico
	windres $< -O coff -o $@

$(RELEASE_OUT): $(RELEASE_OBJ) $(RELEASE_MY_LIB_OBJ) $(RES_OBJ) | bin
	$(CXX) -o $@ $^ $(RELEASE_LDFLAGS)

$(LIB_NAME): $(MY_LIB_OBJ) | my_lib/lib
	ar rcs $@ $^

# Debug build object files
obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

my_lib/obj/%.o: my_lib/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Release build object files
release_obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(RELEASE_CXXFLAGS) -c $< -o $@

release_obj/my_lib_%.o: my_lib/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(RELEASE_CXXFLAGS) -c $< -o $@

# Mappen aanmaken
bin obj my_lib/obj my_lib/lib release_obj:
	mkdir -p $@

# Runnen
run: all
	./$(OUT)

# Opruimen
clean:
	rm -rf obj my_lib/obj bin/$(notdir $(OUT)) bin/$(notdir $(RELEASE_OUT)) resources/icon.o

fclean: clean
	rm -rf my_lib/lib bin release release.zip
	rm -rf release_obj

re: fclean all

RELEASE_DLLS = \
    /mingw64/bin/libraylib.dll \
    /mingw64/bin/glfw3.dll \
    /mingw64/bin/libwinpthread-1.dll \
    /mingw64/bin/libgcc_s_seh-1.dll \
    /mingw64/bin/libstdc++-6.dll

release: $(RELEASE_OUT)
	rm -rf $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)
	cp $(RELEASE_OUT) $(RELEASE_DIR)/pompion.exe
	cp -r data $(RELEASE_DIR)/
ifeq ($(OS_TYPE), windows)
	cp $(RELEASE_DLLS) $(RELEASE_DIR)/
endif

zip: release
	@which zip > /dev/null 2>&1 || (echo "zip not found. Install with: pacman -S zip" && exit 1)
	zip -r release.zip $(RELEASE_DIR)

.PHONY: all run clean fclean re release zip