# ======================
#  Build Configuration
# ======================
PARAMS_HEAD = 128s
THASH = simple
PRINT = PRINT

PARAMS := sphincs-SM3-$(PARAMS_HEAD)

# ================
#  Include Paths
# ================
INCLUDE_DIR    = include
NETWORK_DIR    = network
INCLUDE_SUBDIRS = hash signature params utils thread crypto
NETWORK_SUBDIRS = handler connection threadpool keygen sign round data
INCLUDE_FLAGS  = -I$(INCLUDE_DIR) $(addprefix -I$(INCLUDE_DIR)/, $(INCLUDE_SUBDIRS)) \
      -I$(NETWORK_DIR) $(addprefix -I$(NETWORK_DIR)/, $(NETWORK_SUBDIRS)) \
      -I/usr/include/cjson \
      -D$(PRINT) \

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

TSS_HEADERS  = $(HEADERS)

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

NET_HEADERS = $(TSS_HEADERS) \
              $(wildcard network/*/*.h network/*.h)

# ======================
#  API Shared Library
# ======================
API_OUTPUT_DIR = network/Web
API_TARGET = $(API_OUTPUT_DIR)/libtssapi.so
API_SOURCES = $(NET_SOURCES) \
              network/Web/Web_api.c

API_HEADERS = $(NET_HEADERS)


# ================
#  Build Targets
# ================
.PHONY: all clean tss nets print_params api

all: tss api nets

# -- TSS Target --
TSS_BIN = multi_thread_test/TSS-multi-thread
$(TSS_BIN): multi_thread_test/TSS-multi-thread.c $(TSS_SOURCES) $(TSS_HEADERS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -- Network Targets --
NET_TARGETS = network/keygen/client network/keygen/TTP network/sign/client network/sign/TTP network/verify/verify
$(NET_TARGETS): %: %.c $(NET_SOURCES) $(NET_HEADERS)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -o $@ $< $(filter-out $<,$^) $(LDLIBS)

# -- API Shared Library --
API_CFLAGS = $(CFLAGS) -fPIC -fvisibility=hidden
LDFLAGS = -Wl,--exclude-libs,ALL

$(API_TARGET): $(API_SOURCES) $(API_HEADERS)
	@echo "Building TSS API shared library..."
	@$(CC) $(API_CFLAGS) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

# ===================
#  Execution Helpers
# ===================

%.exec: %
	@./$<

tss: print_params $(TSS_BIN).exec

nets: $(NET_TARGETS)

api: $(API_TARGET)

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
	@-$(RM) $(TSS_BIN)
	@-$(RM) $(NET_TARGETS)
	@-$(RM) $(API_TARGET)