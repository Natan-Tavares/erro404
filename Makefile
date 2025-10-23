
default:
	gcc -o main.exe src/*.c -I include -L lib -lraylib -lgdi32 -lwinmm
