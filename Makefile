ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    LIBS = -lraylib -lgdi32 -lwinmm
else
    TARGET = main
    LIBS = -lraylib -lm -lpthread -ldl -lrt
endif

INCLUDE = -I include
LIBDIR = -L lib
SRC = $(wildcard src/*.c)
STATIC = lib/libraylib.a
RAYLIB_DIR = raylib

RAYLIB_REPO = https://github.com/raysan5/raylib.git

all: $(TARGET)

$(RAYLIB_DIR):
	git clone $(RAYLIB_REPO)

$(STATIC): $(RAYLIB_DIR)
	mkdir -p lib
	cd $(RAYLIB_DIR)/src && make PLATFORM=PLATFORM_DESKTOP && mv libraylib.a ../../lib/

$(TARGET): $(STATIC) $(SRC)
	gcc -o $(TARGET) src/*.c $(INCLUDE) $(LIBDIR) $(LIBS)
