all: tank tags

tags: 
	exuberant-ctags src/* include/*

clean:
	rm -vf *.o

aux-math.o: src/aux-math.c include/aux-math.h
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/aux-math.c

graphics.o: src/graphics.c include/graphics.h
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/graphics.c

player.o: src/player.c include/player.h
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/player.c

bot.o: src/bot.c include/bot.h
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/bot.c

bullet.o: src/bullet.c include/bullet.h
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/bullet.c

wall.o: src/wall.c include/wall.h
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/wall.c

tank.o: src/tank.c
	gcc -Wall -ggdb -Iinclude -c `sdl-config --cflags` src/tank.c

tank: tank.o player.o graphics.o aux-math.o wall.o bot.o bullet.o
	gcc -Wall -ggdb `sdl-config --libs` tank.o wall.o bullet.o bot.o player.o graphics.o aux-math.o -o tank
