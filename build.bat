gcc -c *.c -Wall
gcc *.o -LOpenGL32.lib -Lglfw3.lib -lm -o game
