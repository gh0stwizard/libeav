CFLAGS ?= -O2 -Wall -Wextra -std=c99 -pedantic -fPIC
PKG_CONFIG ?= pkg-config
DESTDIR ?= /usr/local
INSTALL ?= install

BINDIR ?= $(DESTDIR)/bin
LIBDIR ?= $(DESTDIR)/lib
INCLUDEDIR ?= $(DESTDIR)/include
DATAROOTDIR ?= $(DESTDIR)/share
MANDIR ?= $(DATAROOTDIR)/man

MAJOR_VERSION = 0
MINOR_VERSION = 1
PATCH_VERSION = 1
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)

DLL_EXT ?= so
LIB_EXT ?= a
DLL_TARGET = libeav.$(DLL_EXT)
LIB_TARGET = libeav.$(LIB_EXT)
BIN_TARGET = eav

TARGETS = $(BIN_TARGET) $(DLL_TARGET) $(LIB_TARGET)
bin_SOURCES = $(wildcard bin/*.c)
bin_OBJECTS = $(patsubst bin/%.c, bin/%.o, $(bin_SOURCES))
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, src/%.o, $(SOURCES))

#----------------------------------------------------------#

ifdef FORCE_IDN
ifeq ($(FORCE_IDN),idnkit)
$(info > Force using idnkit)
IDNKIT_DIR ?= /usr/local
DEFS = -DHAVE_IDNKIT -I$(IDNKIT_DIR)/include
LIBS = -L$(IDNKIT_DIR)/lib -lidnkit
PARTIAL = $(wildcard partial/idnkit/*.c)
OBJECTS += $(patsubst  partial/idnkit/%.c,  partial/idnkit/%.o, $(PARTIAL))
else ifeq ($(FORCE_IDN),idn2)
$(info > Force using libidn2)
DEFS = -DHAVE_LIBIDN2 $(shell $(PKG_CONFIG) --cflags libidn2)
LIBS = $(shell $(PKG_CONFIG) --libs libidn2)
PARTIAL = $(wildcard partial/idn2/*.c)
OBJECTS += $(patsubst  partial/idn2/%.c,  partial/idn2/%.o, $(PARTIAL))
else ifeq ($(FORCE_IDN),idn)
$(info > Force using libidn)
DEFS = -DHAVE_LIBIDN $(shell $(PKG_CONFIG) --cflags libidn)
LIBS = $(shell $(PKG_CONFIG) --libs libidn)
PARTIAL = $(wildcard partial/idn/*.c)
OBJECTS += $(patsubst  partial/idn/%.c,  partial/idn/%.o, $(PARTIAL))
else
$(error ! Unknown IDN library type)
endif
else
$(info > Looking for idn library ...)
ifeq ($(shell $(PKG_CONFIG) --exists 'libidn2 >= 2.0.3' || echo NO),)
$(info > Found libidn2)
DEFS = -DHAVE_LIBIDN2 $(shell $(PKG_CONFIG) --cflags libidn2)
LIBS = $(shell $(PKG_CONFIG) --libs libidn2)
PARTIAL = $(wildcard partial/idn2/*.c)
OBJECTS += $(patsubst  partial/idn2/%.c,  partial/idn2/%.o, $(PARTIAL))
else ifeq ($(shell $(PKG_CONFIG) --exists 'libidn' || echo NO),)
$(info > Found libidn)
DEFS = -DHAVE_LIBIDN $(shell $(PKG_CONFIG) --cflags libidn)
LIBS = $(shell $(PKG_CONFIG) --libs libidn)
PARTIAL = $(wildcard partial/idn/*.c)
OBJECTS += $(patsubst  partial/idn/%.c,  partial/idn/%.o, $(PARTIAL))
else
$(info > Using idnkit by default)
IDNKIT_DIR ?= /usr/local
DEFS = -DHAVE_IDNKIT -I$(IDNKIT_DIR)/include
LIBS = -L$(IDNKIT_DIR)/lib -lidnkit
PARTIAL = $(wildcard partial/idnkit/*.c)
OBJECTS += $(patsubst  partial/idnkit/%.c,  partial/idnkit/%.o, $(PARTIAL))
endif
endif

#----------------------------------------------------------#

CPPFLAGS += -Iinclude -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
CPPFLAGS += $(DEFS) $(INCLUDES)

LIB_PATH = $(shell realpath .)

export _defs = $(DEFS)
export _libs = $(LIBS)
export _pkg_config = $(PKG_CONFIG)
export _install = $(INSTALL)
export _idnkit_dir = $(IDNKIT_DIR)
export _destdir = $(DESTDIR)
export _objects = $(OBJECTS)
export _libpath = $(LIB_PATH)

#----------------------------------------------------------#

all: depend libs app

debug: CFLAGS += -g -D_DEBUG
debug: all

libs: shared static
shared: $(DLL_TARGET)
static: $(LIB_TARGET)

app: $(BIN_TARGET)

$(BIN_TARGET): $(DLL_TARGET)
	$(MAKE) -C bin

$(DLL_TARGET): $(OBJECTS)
	# library -> shared linkage
	$(CC) -shared $(LDFLAGS) -Iinclude -Wl,-soname,$(DLL_TARGET) -o $(DLL_TARGET) $(OBJECTS) $(LIBS)

$(LIB_TARGET): $(OBJECTS)
	# library -> static linkage
	$(AR) rcs $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. -o $@ -c $<

check: $(TARGETS)
	$(MAKE) -C tests check IDNKIT_DIR=$(IDNKIT_DIR)

man:
	$(MAKE) -C docs VERSION=$(VERSION)

#----------------------------------------------------------#

clean: clean-tests clean-bin
	# cleanup
	$(RM) $(DLL_TARGET) $(LIB_TARGET) $(OBJECTS) Makefile.depend

clean-tests:
	$(MAKE) -C tests clean
	
clean-bin:
	$(MAKE) -C bin clean

strip: $(TARGETS)
	$(MAKE) -C bin strip

#----------------------------------------------------------#

install: install-bin install-libs install-man

install-bin: $(BIN_TARGET)
	$(MAKE) -C bin install DESTDIR=$(DESTDIR)

install-libs: install-shared install-static

install-shared: $(DLL_TARGET)
	# installing shared library
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) -d $(INCLUDEDIR)
	$(INSTALL) -m 0644 include/eav.h $(INCLUDEDIR)
	$(INSTALL) -m 0755 $(DLL_TARGET) $(LIBDIR)/$(DLL_TARGET).$(VERSION)
	cd $(LIBDIR) && \
	ln -snf $(DLL_TARGET).$(VERSION) $(DLL_TARGET).$(MAJOR_VERSION) && \
	ln -snf $(DLL_TARGET).$(VERSION) $(DLL_TARGET)

install-static: $(LIB_TARGET)
	# installing static library
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) -d $(INCLUDEDIR)
	$(INSTALL) -m 0644 include/eav.h $(INCLUDEDIR)
	$(INSTALL) -m 0644 $(LIB_TARGET) $(LIBDIR)

install-man:
	$(INSTALL) -d $(MANDIR)/man3
	$(INSTALL) -m 0644 docs/libeav.3.gz $(MANDIR)/man3

#----------------------------------------------------------#

.PHONY: all debug check clean docs install libs

depend: Makefile.depend
Makefile.depend:
	$(CC) $(CFLAGS) -MM -MG $(SOURCES) > $@

-include Makefile.depend

.DELETE_ON_ERROR:
