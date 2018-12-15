all: client server Menu block_game2

client: client.c
	gcc -o client client.c

server: server.c
	gcc -o server server.c

Menu: Menu.c
	gcc -o Menu Menu.c

block_game2: block_game2.c
	gcc -o block_game2 block_game2.c -lncurses

clean:
	rm client server Menu block_game2
