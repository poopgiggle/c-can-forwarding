CC=gcc
XENO_CONFIG:=/usr/bin/xeno-config
XENO_POSIX_CFLAGS:=$(shell $(XENO_CONFIG) --skin=posix --cflags)
XENO_POSIX_LIBS:=$(shell $(XENO_CONFIG) --skin=posix --ldflags)
INSTALL_PREFIX:=/opt/synercon

all: fwd-can.c
	$(CC) -pthread -o fwd-can $(XENO_POSIX_CFLAGS) $(XENO_POSIX_LIBS) -O4 fwd-can.c

clean:
	rm fwd-can

install:
	cp fwd-can $(DESTDIR)$(INSTALL_PREFIX)/bin



