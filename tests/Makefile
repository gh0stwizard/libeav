IDNKIT_DIR ?= /usr/local
IDNKIT_CFLAGS ?= -I$(IDNKIT_DIR)/include
IDNKIT_LIBS ?= -L$(IDNKIT_DIR)/lib -lidnkit

LIB_DIRS = $(IDNKIT_DIR)/lib

# for test the library only
LIB_PATH = $(LIB_DIRS):$(shell realpath ..)

CFLAGS ?= -Wall -Wextra -std=c99 -pedantic
CFLAGS += -I../include
CFLAGS += $(IDNKIT_CFLAGS)
CFLAGS += -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
LDFLAGS ?= 
LIBS ?= 
LIBS += $(IDNKIT_LIBS)

HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
TARGETS = $(patsubst %.c, %.bin, $(SOURCES))

DEP_SRC = $(wildcard ../src/*.c)
DEP_OBJ = $(patsubst %.c, %.o, $(DEP_SRC))

all: $(TARGETS)

debug: CFLAGS += -g -D_DEBUG
debug: all

%.bin: %.o
	# linkage
	$(CC) $(LDFLAGS) -o $@ $< $(DEP_OBJ) $(LIBS)
	
%.o: %.c $(HEADERS)
	# compile
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	# cleanup
	$(RM) $(TARGETS) $(OBJECTS)

check: $(TARGETS)
	# all TLDs test
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-all-tlds-test.bin ../data/tld-domains.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-domain-length.bin ../data/domain-length.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-dash-domains.bin ../data/xn-dash-domains.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-822-localpart.bin ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5321-localpart.bin ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5322-localpart.bin ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-localpart.bin 20 22 ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-localpart.bin 13 11 ../data/localpart-utf8.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-822-email.bin 32 23 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5321-email.bin 29 26 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5322-email.bin 26 29 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-fqdn.bin 28 27 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-fqdn.bin 4 7 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email.bin 28 27 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email.bin 8 3 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
	LD_LIBRARY_PATH=$(LIB_PATH) ./t-eav.bin 8 3 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_PATH) ./t-eav.bin 7 1 ../data/email-reg.ru.txt

.PHONY: all debug clean check
