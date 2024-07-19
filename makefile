all:
	gtk4-builder-tool simplify --3to4 window2.glade > window.ui
	gcc `pkg-config --cflags gtk4` main.c -o app `pkg-config --libs gtk4`
