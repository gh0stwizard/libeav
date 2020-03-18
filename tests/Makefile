CFLAGS ?= -O2 -Wall -Wextra -std=c99 -pedantic

DEFS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500 -D_SVID_SOURCE
DEFS += -I../include -I. ${_defs}

# used for test the library only
LIB_PATH = $(shell realpath ..)

#----------------------------------------------------------#

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
TARGETS = $(patsubst %.c, %.bin, $(SOURCES))

$(info >>> Building tests with idn = ${_withidn})
ifeq (${_withidn},idnkit)
src_pattern = idnkit/%.c
PARTIAL_SRC = $(wildcard idnkit/*.c)
PARTIAL_OBJ = $(patsubst idnkit/%.c, %.o, $(PARTIAL_SRC))
PARTIAL_TRT = $(patsubst idnkit/%.c, %.bin, $(PARTIAL_SRC))
else
src_pattern = idnX/%.c
PARTIAL_SRC = $(wildcard idnX/*.c)
PARTIAL_OBJ = $(patsubst idnX/%.c, %.o, $(PARTIAL_SRC))
PARTIAL_TRT = $(patsubst idnX/%.c, %.bin, $(PARTIAL_SRC))
endif

DEP_OBJ = $(patsubst %.o, ../%.o, ${_objects})

LIBS = ${_libs} -L.. -leav

$(info !!! sources $(SOURCES))
$(info !!! objects $(OBJECTS))
$(info !!! targets $(TARGETS))

#----------------------------------------------------------#

CPPFLAGS += $(DEFS) $(INCLUDES)
export LD_LIBRARY_PATH=$(LIB_PATH)

all: depend $(TARGETS) $(PARTIAL_TRT)

%.bin: %.o
	# === tests -> linkage
	$(CC) $(LDFLAGS) -o $@ $< $(DEP_OBJ) $(LIBS)

%.o: %.c
	# === tests -> compile $<
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<
	
%.o: $(src_pattern)
	# === tests -> compile $<
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

clean:
	# === tests -> cleanup
	$(RM) $(TARGETS) $(OBJECTS) $(PARTIAL_OBJ) $(PARTIAL_TRT) Makefile.depend

check: $(TARGETS) $(PARTIAL_TRT) check-domain check-lpart check-email check-lib

check-domain:
	# =====================================================
	# domain tests
	# =====================================================
	./t-all-tlds-test.bin ../data/tld-domains.txt
	./t-domain-length.bin ../data/domain-length.txt
	./t-dash-domains.bin ../data/xn-dash-domains.txt

check-lpart:
	# localpart tests
	./t-822-localpart.bin ../data/localpart-ascii.txt
	./t-5321-localpart.bin ../data/localpart-ascii.txt
	./t-5322-localpart.bin ../data/localpart-ascii.txt
	./t-6531-localpart.bin 20 22 ../data/localpart-ascii.txt
	./t-6531-localpart.bin 13 11 ../data/localpart-utf8.txt

check-email:
	# email address tests
#	./t-822-email.bin 43 42 ../data/email-ascii.txt
#	./t-5321-email.bin 40 45 ../data/email-ascii.txt
#	./t-5322-email.bin 37 48 ../data/email-ascii.txt
#	./t-6531-email.bin 39 46 ../data/email-ascii.txt
	./t-6531-email.bin 7 17 ../data/email-utf8.txt
	./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
#	./t-6531-email-fqdn.bin 39 46 ../data/email-ascii.txt
	./t-6531-email-fqdn.bin 7 17 ../data/email-utf8.txt
	./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
#	./t-6531-email-tld.bin 29 56 ../data/email-ascii.txt
	./t-6531-email-tld.bin 4 20 ../data/email-utf8.txt
	./t-6531-email-tld.bin 6 2 ../data/email-reg.ru.txt
#	./t-6531-email-tld-special.bin 29 56 ../data/email-ascii.txt

check-lib:
	# library tests
	./t-eav.bin 4 20 ../data/email-utf8.txt
	./t-eav.bin 7 1 ../data/email-reg.ru.txt
#	./t-eav-all-modes.bin 198 227 ../data/email-ascii.txt

.PHONY: all debug clean check

#----------------------------------------------------------#

depend: Makefile.depend
Makefile.depend:
	$(CC) $(CFLAGS) -MM -MG $(SOURCES) $(PARTIAL_SRC) > $@

-include Makefile.depend

.DELETE_ON_ERROR:
