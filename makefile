all:
	gcc `pkg-config --cflags gtk4` main.c -o app `pkg-config --libs gtk4`

