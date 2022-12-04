make:
	g++ main.cpp glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o executable
	./executable
