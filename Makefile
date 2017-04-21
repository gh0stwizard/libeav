DESTDIR ?= /usr/local

IDNKIT_DIR ?= /usr/local
IDNKIT_CFLAGS ?= -I$(IDNKIT_DIR)/include
IDNKIT_LIBS ?= -L$(IDNKIT_DIR)/lib -lidnkit

MY_CFLAGS = -Wall -Wextra -std=c99 -pedantic -fPIC
MY_CFLAGS += -Iinclude
MY_CFLAGS += $(IDNKIT_CFLAGS)
MY_CFLAGS += -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
MY_CFLAGS += $(CFLAGS)
LDFLAGS ?= -shared
LIBS ?= 
LIBS += $(IDNKIT_LIBS)

SO_TARGET = libeav.so.$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)
TARGET_LIBS = $(SO_TARGET) libeav.a
TARGETS = $(TARGET_LIBS) eav
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

MAJOR_VERSION = 0
MINOR_VERSION = 1
PATCH_VERSION = 0


all: $(TARGETS)

debug: MY_CFLAGS += -g -D_DEBUG
debug: all
	cd tests; $(MAKE) clean debug

$(TARGETS): $(OBJECTS)

libs: $(TARGET_LIBS)

eav:
	cd bin; $(MAKE)

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
	cd tests; $(MAKE) clean
	
clean-bin:
	cd bin; $(MAKE) clean

strip: $(TARGETS) strip-bin
	# strip
	strip --strip-unneeded -R .comment -R .note -R .note.ABI-tag $(TARGETS)

strip-bin:
	cd bin; $(MAKE) strip

check: $(OBJECTS)
	cd tests; $(MAKE) check IDNKIT_DIR=$(IDNKIT_DIR)

install: $(TARGETS)
	mkdir -p $(DESTDIR)/bin $(DESTDIR)/lib
	cp bin/eav $(DESTDIR)/bin
	cp libeav.a $(SO_TARGET) $(DESTDIR)/lib
	cd $(DESTDIR)/lib && \
	ln -sf $(SO_TARGET) libeav.so.$(MAJOR_VERSION) && \
	ln -sf $(SO_TARGET) libeav.so

.PHONY: all debug check clean install libs
