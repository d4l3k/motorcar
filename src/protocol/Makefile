MOTORCAR_PROTOCOL_XML=motorcar.xml
XDG_PROTOCOL_XML=xdg-shell.xml

motorcar-protocol: $(MOTORCAR_PROTOCOL_XML)
	wayland-scanner client-header < $(MOTORCAR_PROTOCOL_XML) > motorcar-client-protocol.h
	wayland-scanner server-header < $(MOTORCAR_PROTOCOL_XML) > motorcar-server-protocol.h
	wayland-scanner code < $(MOTORCAR_PROTOCOL_XML) > motorcar-wayland-extensions.c
	wayland-scanner client-header < $(XDG_PROTOCOL_XML) > xdg-shell-client-protocol.h
	wayland-scanner server-header < $(XDG_PROTOCOL_XML) > xdg-shell-server-protocol.h
	wayland-scanner code < $(XDG_PROTOCOL_XML) > xdg-shell-protocol.c
	gcc -c -Wall -Werror -fpic motorcar-wayland-extensions.c
	gcc -c -Wall -Werror -fpic xdg-shell-protocol.c
	gcc -shared -o libmotorcar-server.so motorcar-wayland-extensions.o xdg-shell-protocol.o

clean:
	rm -rf  ./*.h ./*.c ./*.o ./*.so
