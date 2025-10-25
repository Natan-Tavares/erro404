
default:
	gcc -o main.exe src/tilemap.c -I include -L lib -lraylib -lgdi32 -lwinmm
