IDNKIT_DIR ?= /usr/local
IDNKIT_CFLAGS ?= -I$(IDNKIT_DIR)/include
IDNKIT_LIBS ?= -L$(IDNKIT_DIR)/lib -lidnkit

EAV_CFLAGS ?= -I../include
EAV_LIBS ?= -L.. -leav

MY_CFLAGS = -Wall -Wextra -std=c99 -pedantic
MY_CFLAGS += $(EAV_CFLAGS) $(IDNKIT_CFLAGS)
MY_CFLAGS += -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
MY_CFLAGS += $(CFLAGS)
LDFLAGS ?= 
LIBS ?= 
LIBS += $(EAV_LIBS) $(IDNKIT_LIBS)

CFLAGS ?= 
CFLAGS += -Wall -std=gnu99
CFLAGS += $(MYHTML_CFLAGS) $(CURL_CFLAGS) $(UV_CFLAGS)
CFLAGS += -D_POSIX_C_SOURCE=200809L
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -D_GNU_SOURCE
LDFLAGS ?= 
LIBS ?= 
LIBS += $(MYHTML_LIBS) $(CURL_LIBS) $(UV_LIBS) $(SQLITE_LIBS)

TARGETS = eav eav-static
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: $(TARGETS)

$(TARGETS): $(OBJECTS)

debug: MY_CFLAGS += -g -D_DEBUG
debug: all

static: eav-static
shared: eav

eav:
	# shared linkage
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(MYHTML_LIBS) $(LIBS)

eav-static: LDFLAGS = -static
eav-static: $(OBJECTS)
	# static linkage
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) -c $(MY_CFLAGS) -o $@ $<

clean:
	# cleanup
	$(RM) $(TARGETS) $(OBJECTS)

strip: $(TARGETS)
	# strip
	strip --strip-unneeded -R .comment -R .note -R .note.ABI-tag $(TARGETS)

.PHONY: all clean debug
