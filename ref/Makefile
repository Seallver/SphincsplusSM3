PARAMS = sphincs-SM3-256f
#(simple or robust)
THASH = robust


CC=/usr/bin/gcc
CFLAGS=-Wall -Wextra -Wpedantic -O3 -std=c99 -Wconversion -Wmissing-prototypes -DPARAMS=$(PARAMS) $(EXTRA_CFLAGS)

SOURCES =          address.c randombytes.c merkle.c wots.c wotsx1.c utils.c utilsx1.c fors.c sign.c
HEADERS = params.h address.h randombytes.h merkle.h wots.h wotsx1.h utils.h utilsx1.h fors.h api.h  hash.h thash.h

ifneq (,$(findstring SM3,$(PARAMS)))
	SOURCES += SM3.c hash_SM3.c thash_SM3_$(THASH).c
	HEADERS += SM3.h
endif


DET_SOURCES = $(SOURCES:randombytes.%=rng.%)
DET_HEADERS = $(HEADERS:randombytes.%=rng.%)

TESTS =         test/fors \
		test/spx \

BENCHMARK = test/benchmark

.PHONY: clean test benchmark

default: PQCgenKAT_sign

all: PQCgenKAT_sign tests benchmarks

tests: $(TESTS)

test: print_params $(TESTS:=.exec)

benchmarks: $(BENCHMARK)

benchmark: print_params $(BENCHMARK:=.exec)

PQCgenKAT_sign: PQCgenKAT_sign.c $(DET_SOURCES) $(DET_HEADERS)
	$(CC) $(CFLAGS) -o $@ $(DET_SOURCES) $< -lcrypto

test/benchmark: test/benchmark.c test/cycles.c $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ test/cycles.c $(SOURCES) $< $(LDLIBS)

test/%: test/%.c $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $< $(LDLIBS)

test/%.exec: test/%
	@$<

clean:
	-$(RM) $(TESTS)
	-$(RM) $(BENCHMARK)
	-$(RM) PQCgenKAT_sign
	-$(RM) PQCsignKAT_*.rsp
	-$(RM) PQCsignKAT_*.req

print_params:
	@params_len=$$(echo -n "$(PARAMS)" | wc -c); \
	thash_len=$$(echo -n "$(THASH)" | wc -c); \
	border_len=$$((params_len + 19)); \
	printf "%0.s=" $$(seq 1 $$border_len); echo ""; \
	printf "||   PARAMS = %-$${params_len}s   ||\n" "$(PARAMS)"; \
	printf "||   THASH  = %-$${params_len}s   ||\n" "$(THASH)"; \
	printf "%0.s=" $$(seq 1 $$border_len); echo ""