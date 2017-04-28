DESTDIR ?= /usr/local
BINDIR ?= $(DESTDIR)/bin
LIBDIR ?= $(DESTDIR)/lib
DATAROOTDIR ?= $(DESTDIR)/share
MANDIR ?= $(DATAROOTDIR)/man

IDNKIT_DIR ?= /usr/local
IDNKIT_CFLAGS ?= -I$(IDNKIT_DIR)/include
IDNKIT_LIBS ?= -L$(IDNKIT_DIR)/lib -lidnkit

MY_CFLAGS = -Wall -Wextra -std=c99 -pedantic -fPIC
MY_CFLAGS += -Iinclude
MY_CFLAGS += $(IDNKIT_CFLAGS)
MY_CFLAGS += -D_DEFAULT_SOURCE
MY_CFLAGS += -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
MY_CFLAGS += $(CFLAGS)
LDFLAGS ?= -shared
LIBS ?= 
LIBS += $(IDNKIT_LIBS)

SO_TARGET = libeav.so.$(VERSION)
TARGET_LIBS = $(SO_TARGET) libeav.a
TARGETS = $(TARGET_LIBS) eav
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

# XXX
MAJOR_VERSION = 0
MINOR_VERSION = 1
PATCH_VERSION = 0
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)


all: $(TARGETS)

debug: MY_CFLAGS += -g -D_DEBUG
debug: all

$(TARGETS): $(OBJECTS)

libs: $(TARGET_LIBS)

eav:
	$(MAKE) -C bin

libeav.so: $(SO_TARGET)

$(SO_TARGET): $(OBJECTS)
	# shared linkage
	$(CC) $(LDFLAGS) -Wl,-soname,$(SO_TARGET) -o $(SO_TARGET) $(OBJECTS) $(LIBS)

libeav.a: $(OBJECTS)
	# static linkage
	ar rcs $@ $(OBJECTS)

%.o: %.c
	$(CC) -c $(MY_CFLAGS) -o $@ $<

clean: clean-tests clean-bin
	# cleanup
	$(RM) $(TARGETS) $(SO_TARGET) $(OBJECTS)

clean-tests:
	$(MAKE) -C tests clean
	
clean-bin:
	$(MAKE) -C bin clean

clean-docs: clean-man-pages

clean-man-pages:
	$(MAKE) -C docs clean

strip: $(TARGETS) strip-bin
	# strip
	strip --strip-unneeded -R .comment -R .note -R .note.ABI-tag $(TARGETS)

strip-bin:
	$(MAKE) -C bin strip

check: $(TARGETS)
	$(MAKE) -C tests check IDNKIT_DIR=$(IDNKIT_DIR)

docs: man-pages

man-pages:
	$(MAKE) -C docs VERSION=$(VERSION)

install: $(TARGETS) man-pages
	mkdir -p $(BINDIR) $(LIBDIR) $(MANDIR)/man3
	cp bin/eav $(BINDIR)
	cp libeav.a $(SO_TARGET) $(LIBDIR)
	cd $(LIBDIR) && \
	ln -sf $(SO_TARGET) libeav.so.$(MAJOR_VERSION) && \
	ln -sf $(SO_TARGET) libeav.so
	cp docs/libeav.3.gz $(MANDIR)/man3

.PHONY: all debug check clean docs install libs
