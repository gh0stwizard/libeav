CFLAGS ?= -O2
CPPFLAGS += -Wall -Wextra -std=c99 -pedantic
CPPFLAGS += -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700 -D_SVID_SOURCE -D__EXTENSIONS__
CPPFLAGS += -I../include -I.
DEFS = ${_defs}

# used for test the library only
LIB_PATH = $(shell realpath .. 2>/dev/null || readlink -f -- ..)
ifeq ($(LIB_PATH),)
$(error Neither realpath or readlink has been found on the system)
endif
export LD_LIBRARY_PATH=$(LIB_PATH)

#----------------------------------------------------------#

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
TARGETS = $(patsubst %.c, %.bin, $(SOURCES))

# coverage data per test binary
gcov_gcda = $(patsubst %.c, %.gcda, $(SOURCES))
gcov_gcno = $(patsubst %.c, %.gcno, $(SOURCES))

$(info >>> Building tests with idn = ${_withidn})
ifeq (${_withidn},idnkit)
src_pattern = idnkit/%.c
PARTIAL_SRC = $(wildcard idnkit/*.c)
PARTIAL_OBJ = $(patsubst idnkit/%.c, %.o, $(PARTIAL_SRC))
PARTIAL_TRT = $(patsubst idnkit/%.c, %.bin, $(PARTIAL_SRC))
LIB_PATH = $(shell realpath ..):${_idnkit_dir}/lib
gcov_gcda += $(patsubst idnkit/%.c, %.gcda, $(PARTIAL_SRC))
gcov_gcno += $(patsubst idnkit/%.c, %.gcno, $(PARTIAL_SRC))
else
src_pattern = idnX/%.c
PARTIAL_SRC = $(wildcard idnX/*.c)
PARTIAL_OBJ = $(patsubst idnX/%.c, %.o, $(PARTIAL_SRC))
PARTIAL_TRT = $(patsubst idnX/%.c, %.bin, $(PARTIAL_SRC))
gcov_gcda += $(patsubst idnX/%.c, %.gcda, $(PARTIAL_SRC))
gcov_gcno += $(patsubst idnX/%.c, %.gcno, $(PARTIAL_SRC))
endif

DEP_OBJ = $(patsubst %.o, ../%.o, ${_objects})
LIBS = ${_libs}

#----------------------------------------------------------#

all: $(TARGETS) $(PARTIAL_TRT)

%.bin: %.o
	# === tests -> linkage
	$(CC) -o $@ $< $(DEP_OBJ) -L.. -leav $(LIBS) $(LDFLAGS)

%.o: %.c
	# === tests -> compile $<
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $@ $<
	
%.o: $(src_pattern)
	# === tests -> compile $<
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $@ $<

clean:
	# === tests -> cleanup
	$(RM) $(TARGETS) $(OBJECTS) $(PARTIAL_OBJ) $(PARTIAL_TRT)
	$(RM) $(gcov_gcda)
	$(RM) $(gcov_gcno)

check: check-domain check-lpart check-email check-lib

check-domain: t-all-tlds-test.bin t-domain-length.bin
check-domain: t-dash-domains.bin t-retired.bin
	# =====================================================
	# domain tests
	# =====================================================
	./t-all-tlds-test.bin ../data/tld-domains.txt
ifeq (${_withidn},idnkit)
	./t-domain-length.bin 4 5 ../data/domain-length.txt
else ifeq ($(LABELS_ALLOW_UNDERSCORE),ON)
	./t-domain-length.bin 6 3 ../data/domain-length.txt
else
	./t-domain-length.bin 4 5 ../data/domain-length.txt
endif
	./t-dash-domains.bin ../data/xn-dash-domains.txt
	#./t-retired.bin 2 2 ../data/retired.txt

check-lpart: t-822-localpart.bin t-5321-localpart.bin
check-lpart: t-5322-localpart.bin t-6531-localpart.bin
	# === localpart tests
	./t-822-localpart.bin 23 20 ../data/localpart-ascii.txt
	./t-5321-localpart.bin 21 22 ../data/localpart-ascii.txt
	./t-5322-localpart.bin 23 20 ../data/localpart-ascii.txt
ifeq ($(RFC6531_FOLLOW_RFC5322),ON)
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	#
	# --- RFC6531_FOLLOW_RFC5322 = ON ; RFC6531_FOLLOW_RFC20 = ON
	#
	./t-6531-localpart.bin 22 21 ../data/localpart-ascii.txt
	./t-6531-localpart.bin 15 2 ../data/localpart-utf8.txt
else
	#
	# --- RFC6531_FOLLOW_RFC5322 = ON ; RFC6531_FOLLOW_RFC20 = OFF
	#
	./t-6531-localpart.bin 23 20 ../data/localpart-ascii.txt
	./t-6531-localpart.bin 15 2 ../data/localpart-utf8.txt
endif
else
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	#
	# --- RFC6531_FOLLOW_RFC5322 = OFF ; RFC6531_FOLLOW_RFC20 = ON
	#
	./t-6531-localpart.bin 20 23 ../data/localpart-ascii.txt
	./t-6531-localpart.bin 13 4 ../data/localpart-utf8.txt
else
	#
	# --- RFC6531_FOLLOW_RFC5322 = OFF ; RFC6531_FOLLOW_RFC20 = OFF
	#
	./t-6531-localpart.bin 21 22 ../data/localpart-ascii.txt
	./t-6531-localpart.bin 13 4 ../data/localpart-utf8.txt
endif
endif
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	./t-6531-localpart.bin 0 7 ../data/localpart-utf8-rfc20.txt
else
	./t-6531-localpart.bin 7 0 ../data/localpart-utf8-rfc20.txt
endif


check-email: check-email-822 check-email-5321
check-email: check-email-5322 check-email-6531


check-email-822: t-822-email.bin
	# === RFC 822 email address tests
	./t-822-email.bin -P 45 -F 1  -f ../data/pass-email-ascii.txt
	./t-822-email.bin -P 0  -F 44 -f ../data/fail-email-ascii.txt
	./t-822-email.bin -P 5 -F 0 -s -f ../data/pass-email-ascii-slurp.txt
	./t-822-email.bin -P 0 -F 7 -s -f ../data/fail-email-ascii-slurp.txt


check-email-5321: t-5321-email.bin
	# === RFC 5321 email address tests
	./t-5321-email.bin 40 6 ../data/pass-email-ascii.txt
	./t-5321-email.bin 0 44 ../data/fail-email-ascii.txt


check-email-5322: t-5322-email.bin
	# === RFC 5322 email address tests
	./t-5322-email.bin 44 2 ../data/pass-email-ascii.txt
	./t-5322-email.bin 0 44 ../data/fail-email-ascii.txt


check-email-6531: t-6531-email.bin t-6531-email-fqdn.bin
check-email-6531: t-6531-email-tld.bin t-6531-email-tld-special.bin
	# === RFC 6531 tests
ifeq ($(RFC6531_FOLLOW_RFC5322),ON)
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	#
	# --- RFC6531_FOLLOW_RFC5322 = ON ; RFC6531_FOLLOW_RFC20 = ON
	#
	./t-6531-email.bin 43 3 ../data/pass-email-ascii.txt
	./t-6531-email.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email.bin 8 16 ../data/email-utf8.txt
else
	./t-6531-email.bin 7 17 ../data/email-utf8.txt
endif
	./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-fqdn.bin 43 3 ../data/pass-email-ascii.txt
	./t-6531-email-fqdn.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-fqdn.bin 8 16 ../data/email-utf8.txt
else
	./t-6531-email-fqdn.bin 7 17 ../data/email-utf8.txt
endif
	./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-tld.bin 33 13 ../data/pass-email-ascii.txt
	./t-6531-email-tld.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-tld.bin 5 19 ../data/email-utf8.txt
else
	./t-6531-email-tld.bin 4 20 ../data/email-utf8.txt
endif
	./t-6531-email-tld.bin 6 2 ../data/email-reg.ru.txt
	./t-6531-email-tld-special.bin 33 13 ../data/pass-email-ascii.txt
	./t-6531-email-tld-special.bin 0 44 ../data/fail-email-ascii.txt
else
	# --- RFC6531_FOLLOW_RFC5322 = ON ; RFC6531_FOLLOW_RFC20 = OFF
	./t-6531-email.bin 44 2 ../data/pass-email-ascii.txt
	./t-6531-email.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email.bin 12 12 ../data/email-utf8.txt
else
	./t-6531-email.bin 11 13 ../data/email-utf8.txt
endif
	./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-fqdn.bin 44 2 ../data/pass-email-ascii.txt
	./t-6531-email-fqdn.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-fqdn.bin 12 12 ../data/email-utf8.txt
else
	./t-6531-email-fqdn.bin 11 13 ../data/email-utf8.txt
endif
	./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-tld.bin 34 12 ../data/pass-email-ascii.txt
	./t-6531-email-tld.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-tld.bin 5 19 ../data/email-utf8.txt
else
	./t-6531-email-tld.bin 4 20 ../data/email-utf8.txt
endif
	./t-6531-email-tld.bin 6 2 ../data/email-reg.ru.txt
	./t-6531-email-tld-special.bin 34 12 ../data/pass-email-ascii.txt
	./t-6531-email-tld-special.bin 0 44 ../data/fail-email-ascii.txt
endif
else
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	# --- RFC6531_FOLLOW_RFC5322 = OFF ; RFC6531_FOLLOW_RFC20 = ON
	./t-6531-email.bin 39 7 ../data/pass-email-ascii.txt
	./t-6531-email.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email.bin 8 16 ../data/email-utf8.txt
else
	./t-6531-email.bin 7 17 ../data/email-utf8.txt
endif
	./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-fqdn.bin 39 7 ../data/pass-email-ascii.txt
	./t-6531-email-fqdn.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-fqdn.bin 8 16 ../data/email-utf8.txt
else
	./t-6531-email-fqdn.bin 7 17 ../data/email-utf8.txt
endif
	./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-tld.bin 29 17 ../data/pass-email-ascii.txt
	./t-6531-email-tld.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-tld.bin 5 19 ../data/email-utf8.txt
else
	./t-6531-email-tld.bin 4 20 ../data/email-utf8.txt
endif
	./t-6531-email-tld.bin 6 2 ../data/email-reg.ru.txt
	./t-6531-email-tld-special.bin 29 17 ../data/pass-email-ascii.txt
	./t-6531-email-tld-special.bin 0 44 ../data/fail-email-ascii.txt
else
	# --- RFC6531_FOLLOW_RFC5322 = OFF ; RFC6531_FOLLOW_RFC20 = OFF
	./t-6531-email.bin 40 6 ../data/pass-email-ascii.txt
	./t-6531-email.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email.bin 12 12 ../data/email-utf8.txt
else
	./t-6531-email.bin 11 13 ../data/email-utf8.txt
endif
	./t-6531-email.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-fqdn.bin 40 6 ../data/pass-email-ascii.txt
	./t-6531-email-fqdn.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-fqdn.bin 12 12 ../data/email-utf8.txt
else
	./t-6531-email-fqdn.bin 11 13 ../data/email-utf8.txt
endif
	./t-6531-email-fqdn.bin 7 1 ../data/email-reg.ru.txt
	./t-6531-email-tld.bin 30 16 ../data/pass-email-ascii.txt
	./t-6531-email-tld.bin 0 44 ../data/fail-email-ascii.txt
ifeq (${_withidn},idn)
	./t-6531-email-tld.bin 5 19 ../data/email-utf8.txt
else
	./t-6531-email-tld.bin 4 20 ../data/email-utf8.txt
endif
	./t-6531-email-tld.bin 6 2 ../data/email-reg.ru.txt
	./t-6531-email-tld-special.bin 30 16 ../data/pass-email-ascii.txt
	./t-6531-email-tld-special.bin 0 44 ../data/fail-email-ascii.txt
endif
endif

check-lib: t-eav.bin t-eav-all-modes.bin t-result.bin
	# === library tests
ifeq ($(RFC6531_FOLLOW_RFC5322),ON)
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	#
	# --- RFC6531_FOLLOW_RFC5322 = ON ; RFC6531_FOLLOW_RFC20 = ON
	#
ifeq (${_withidn},idn)
	./t-eav.bin 5 19 ../data/email-utf8.txt
else
	./t-eav.bin 4 20 ../data/email-utf8.txt
endif
	./t-eav.bin 7 1 ../data/email-reg.ru.txt
	./t-eav-all-modes.bin 215 15 ../data/pass-email-ascii.txt
	./t-eav-all-modes.bin 0 220 ../data/fail-email-ascii.txt
else
	#
	# --- RFC6531_FOLLOW_RFC5322 = ON ; RFC6531_FOLLOW_RFC20 = OFF
	#
ifeq (${_withidn},idn)
	./t-eav.bin 9 15 ../data/email-utf8.txt
else
	./t-eav.bin 8 16 ../data/email-utf8.txt
endif
	./t-eav.bin 7 1 ../data/email-reg.ru.txt
	./t-eav-all-modes.bin 217 13 ../data/pass-email-ascii.txt
	./t-eav-all-modes.bin 0 220 ../data/fail-email-ascii.txt
endif
else
ifeq ($(RFC6531_FOLLOW_RFC20),ON)
	#
	# --- RFC6531_FOLLOW_RFC5322 = OFF ; RFC6531_FOLLOW_RFC20 = ON
	#
ifeq (${_withidn},idn)
	./t-eav.bin 5 19 ../data/email-utf8.txt
else
	./t-eav.bin 4 20 ../data/email-utf8.txt
endif
	./t-eav.bin 7 1 ../data/email-reg.ru.txt
	./t-eav-all-modes.bin 207 23 ../data/pass-email-ascii.txt
	./t-eav-all-modes.bin 0 220 ../data/fail-email-ascii.txt
else
	#
	# --- RFC6531_FOLLOW_RFC5322 = OFF ; RFC6531_FOLLOW_RFC20 = OFF
	#
ifeq (${_withidn},idn)
	./t-eav.bin 9 15 ../data/email-utf8.txt
else
	./t-eav.bin 8 16 ../data/email-utf8.txt
endif
	./t-eav.bin 7 1 ../data/email-reg.ru.txt
	./t-eav-all-modes.bin 209 21 ../data/pass-email-ascii.txt
	./t-eav-all-modes.bin 0 220 ../data/fail-email-ascii.txt
endif
endif
	./t-result.bin 7 18 ../data/email-result-check.txt
	#
	# --- LABELS_ALLOW_UNDERSCORE test
	#
ifeq (${_withidn},idnkit)
ifeq ($(LABELS_ALLOW_UNDERSCORE),ON)
	./t-eav-all-modes.bin 3 2 ../data/underscore.txt
else
	./t-eav-all-modes.bin 0 5 ../data/underscore.txt
endif
else
ifeq ($(LABELS_ALLOW_UNDERSCORE),ON)
	./t-eav-all-modes.bin 5 0 ../data/underscore.txt
else
	./t-eav-all-modes.bin 0 5 ../data/underscore.txt
endif
endif

.PHONY: all debug clean check

#----------------------------------------------------------#
