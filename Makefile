#Customisable stuff here
LINUX_COMPILER = gcc
WIN32_COMPILER = /usr/bin/i586-mingw32-gcc
WIN32_WINDRES = i586-mingw32-windres
LINUX_ARM_COMPILER = arm-none-linux-gnueabi-gcc
LINUX_PPC_COMPILER = powerpc-unknown-linux-gnu-gcc
FREEBSD60_COMPILER = i686-pc-freebsd6.0-gcc
MACPORT_COMPILER = i686-apple-darwin9-gcc-4.0.1

LIBPURPLE_CFLAGS = $(shell pkg-config --cflags purple)
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-2.0)
WIN32_DEV_DIR = /root/pidgin/win32-dev
WIN32_PIDGIN_DIR = /root/pidgin/pidgin-2.3.0_win32
WIN32_CFLAGS = -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_PIDGIN_DIR}/libpurple/win32 -I${WIN32_PIDGIN_DIR}/pidgin/win32 -I${WIN32_DEV_DIR}/gtk_2_0/include -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_DEV_DIR}/gtk_2_0/lib/glib-2.0/include -I${WIN32_DEV_DIR}/gtk_2_0/lib/gtk-2.0/include -Wno-format
WIN32_LIBS = -L${WIN32_DEV_DIR}/gtk_2_0/lib -L${WIN32_PIDGIN_DIR}/libpurple -L${WIN32_PIDGIN_DIR}/pidgin -lglib-2.0 -lgobject-2.0 -lintl -lpidgin -lpurple -lws2_32 -L. -lgtk-win32-2.0
MACPORT_CFLAGS = -I/opt/local/include/libpurple -I/opt/local/include/glib-2.0 -I/opt/local/lib/glib-2.0/include -I/opt/local/include -arch i386 -arch ppc -dynamiclib -L/opt/local/lib -lpidgin -lpurple -lglib-2.0 -lgobject-2.0 -lintl -lz -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4

DEB_PACKAGE_DIR = ./debdir

SOURCES = \
	pidgin-irc-format.c

#Standard stuff here
.PHONY:	all clean install sourcepackage

all:	clean irc_format.so

install:
	cp irc_format.so ~/.purple/plugins/

uninstall:
	rm ~/.purple/plugins/irc_format.so

clean:
	rm -f irc_format.dll irc_format.so

irc_format.so:	${SOURCES}
	${LINUX_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${GTK_CFLAGS} -I. -g -O2 -pipe ${SOURCES} -o $@ -shared -fPIC -DPIC

irc_format.dll:	${SOURCES}
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O0 -pipe ${SOURCES} -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}
	upx $@
