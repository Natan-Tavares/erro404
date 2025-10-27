
default:
	gcc -o main.exe src/map.c -I include -L lib -lraylib -lgdi32 -lwinmm
