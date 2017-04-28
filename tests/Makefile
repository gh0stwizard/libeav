IDNKIT_DIR ?= /usr/local
IDNKIT_CFLAGS ?= -I$(IDNKIT_DIR)/include
IDNKIT_LIBS ?= -L$(IDNKIT_DIR)/lib -lidnkit

LIB_DIRS = $(IDNKIT_DIR)/lib

# for test the library only
LIB_PATH = $(LIB_DIRS):$(shell realpath ..)

MY_CFLAGS = -Wall -Wextra -std=c99 -pedantic
MY_CFLAGS += -I../include
MY_CFLAGS += $(IDNKIT_CFLAGS)
MY_CFLAGS += -D_DEFAULT_SOURCE
MY_CFLAGS += -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
MY_CFLAGS += $(CFLAGS)
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

debug: MY_CFLAGS += -g -D_DEBUG
debug: all

%.bin: %.o
	# linkage
	$(CC) $(LDFLAGS) -o $@ $< $(DEP_OBJ) $(LIBS)
	
%.o: %.c $(HEADERS)
	# compile
	$(CC) -c $(MY_CFLAGS) -o $@ $<

clean:
	# cleanup
	$(RM) $(TARGETS) $(OBJECTS)

check: $(TARGETS) check-domain check-lpart check-email check-lib

check-domain:
	# domain tests
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-all-tlds-test.bin ../data/tld-domains.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-domain-length.bin ../data/domain-length.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-dash-domains.bin ../data/xn-dash-domains.txt

check-lpart:
	# localpart tests
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-822-localpart.bin ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5321-localpart.bin ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5322-localpart.bin ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-localpart.bin 20 22 ../data/localpart-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-localpart.bin 13 11 ../data/localpart-utf8.txt

check-email:
	# email address tests
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-822-email.bin 42 40 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5321-email.bin 39 43 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-5322-email.bin 36 46 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email.bin 38 44 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email.bin 7 17 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-fqdn.bin 38 44 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-fqdn.bin 7 17 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-tld.bin 28 54 ../data/email-ascii.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-tld.bin 4 20 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-tld.bin 6 2 ../data/email-reg.ru.txt
	LD_LIBRARY_PATH=$(LIB_DIRS) ./t-6531-email-tld-special.bin 28 54 ../data/email-ascii.txt

check-lib:
	# library tests
	LD_LIBRARY_PATH=$(LIB_PATH) ./t-eav.bin 4 20 ../data/email-utf8.txt
	LD_LIBRARY_PATH=$(LIB_PATH) ./t-eav.bin 7 1 ../data/email-reg.ru.txt
	LD_LIBRARY_PATH=$(LIB_PATH) ./t-eav-all-modes.bin 193 217 ../data/email-ascii.txt

.PHONY: all debug clean check
