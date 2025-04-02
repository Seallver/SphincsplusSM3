# ======================
#  Build Configuration
# ======================
PARAMS  = sphincs-SM3-256s
THASH   = simple

# ================
#  Include Paths
# ================
INCLUDE_DIR    = include
NETWORK_DIR    = network
INCLUDE_SUBDIRS = hash signature params utils thread crypto
NETWORK_SUBDIRS = handler connection threadpool keygen sign round data
INCLUDE_FLAGS  = -I$(INCLUDE_DIR) $(addprefix -I$(INCLUDE_DIR)/, $(INCLUDE_SUBDIRS)) \
      -I$(NETWORK_DIR) $(addprefix -I$(NETWORK_DIR)/, $(NETWORK_SUBDIRS)) \
      -I/usr/local/include/cjson

# ================
#  Compiler Setup
# ================
CC      = /usr/bin/gcc
CFLAGS  = -Qn -w -std=c99 -pthread -D_GNU_SOURCE \
          -Wall -Wextra -Wpedantic -Wconversion -Wmissing-prototypes \
          -g -O3 \
          -DPARAMS=$(PARAMS) $(EXTRA_CFLAGS) \
          $(INCLUDE_FLAGS)
LDLIBS  = -lpthread -lcrypto -lcjson

# ================
#  Source Groups
# ================
BASE_SRCDIR = src

# -- Core SPHINCS+ Components --
CORE_SOURCES = $(BASE_SRCDIR)/signature/address.c \
               $(BASE_SRCDIR)/signature/merkle.c \
               $(BASE_SRCDIR)/signature/wots.c \
               $(BASE_SRCDIR)/signature/wotsx1.c \
               $(BASE_SRCDIR)/signature/fors.c \
               $(BASE_SRCDIR)/signature/sign.c

# -- Utility Components --
UTIL_SOURCES = $(BASE_SRCDIR)/utils/utils.c \
               $(BASE_SRCDIR)/utils/utilsx1.c \
               $(BASE_SRCDIR)/utils/randombytes.c

# -- Hash Implementations --
HASH_SOURCES = $(BASE_SRCDIR)/hash/SM3.c \
               $(BASE_SRCDIR)/hash/hash_SM3.c \
               $(BASE_SRCDIR)/hash/thash_SM3_$(THASH).c

SOURCES      = $(CORE_SOURCES) $(UTIL_SOURCES) $(HASH_SOURCES)
HEADERS      = $(wildcard include/*/*.h include/*.h)

# ================
#  TSS Components
# ================
TSS_SOURCES  = $(SOURCES) \
               $(BASE_SRCDIR)/signature/TSS_sign.c \
               $(BASE_SRCDIR)/thread/ThreadSafeQueue.c \
               $(BASE_SRCDIR)/thread/communication.c \
               $(BASE_SRCDIR)/thread/tContext.c \
               $(BASE_SRCDIR)/thread/round.c \
               $(BASE_SRCDIR)/thread/logic.c \
               $(BASE_SRCDIR)/crypto/SSS.c

TSS_HEADERS  = $(HEADERS) include/TSS_api.h

# ================
#  Network Components
# ================
NET_SOURCES  = $(SOURCES) \
               $(BASE_SRCDIR)/crypto/SSS.c \
               $(BASE_SRCDIR)/signature/TSS_sign.c \
               network/handler/keygen_handler.c \
               network/handler/sign_handler.c \
               network/connection/keygen_connection.c \
               network/connection/sign_connection.c  \
               network/threadpool/condition.c \
               network/threadpool/threadpool.c \
               network/round/keygen_round.c \
               network/round/sign_round.c \
               network/data/Json.c

NET_HEADERS = $(HEADERS) include/TSS_api.h \
              $(wildcard network/*/*.h network/*.h)

# ================
#  Build Targets
# ================
.PHONY: all clean test benchmark tss nets print_params

all: PQCgenKAT_sign tests benchmarks

# -- Main Targets --
PQCgenKAT_sign: test/KAT/PQCgenKAT_sign.c $(SOURCES:randombytes%=rng%) 
	@$(CC) $(CFLAGS) -o $@ $^ -lcrypto

# -- Test Targets --
TESTS = test/fors test/spx
$(TESTS): %: %.c $(SOURCES) $(HEADERS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -- Benchmark Target --
BENCHMARK = test/benchmark/benchmark
$(BENCHMARK): $(BENCHMARK).c test/benchmark/cycles.c $(SOURCES) $(HEADERS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -- TSS Target --
TSS_BIN = test/TSS-multi-thread
$(TSS_BIN): test/TSS-multi-thread.c $(TSS_SOURCES) $(TSS_HEADERS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -- Network Targets --
NET_TARGETS = network/keygen/client network/keygen/TTP network/sign/client network/sign/TTP network/verify/verify
$(NET_TARGETS): %: %.c $(NET_SOURCES) $(NET_HEADERS)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -o $@ $< $(filter-out $<,$^) $(LDLIBS)

# ================
#  Execution Helpers
# ================
test: print_params $(TESTS:=.exec)
%.exec: %
	@./$<

benchmark: print_params $(BENCHMARK).exec

tss: print_params $(TSS_BIN).exec

nets: $(NET_TARGETS)

# ================
#  Utilities
# ================
print_params:
	@params_len=$$(echo -n "$(PARAMS)" | wc -c); \
	thash_len=$$(echo -n "$(THASH)" | wc -c); \
	border_len=$$((params_len + 19)); \
	printf "%0.s=" $$(seq 1 $$border_len); echo ""; \
	printf "||   PARAMS = %-$${params_len}s   ||\n" "$(PARAMS)"; \
	printf "||   THASH  = %-$${params_len}s   ||\n" "$(THASH)"; \
	printf "%0.s=" $$(seq 1 $$border_len); echo ""

clean:
	rm -f $(TESTS) $(BENCHMARK) $(TSS_BIN) $(NET_TARGETS) PQCgenKAT_sign \
	      PQCsignKAT_*.rsp PQCsignKAT_*.req