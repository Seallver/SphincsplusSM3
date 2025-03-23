PARAMS = sphincs-SM3-128f
THASH = simple

INCLUDE_DIR = include
INCLUDE_SUBDIRS = hash signature params utils thread crypto

path = $(addprefix -I, $(INCLUDE_DIR) $(foreach dir, $(INCLUDE_SUBDIRS), $(INCLUDE_DIR)/$(dir)))

CC = /usr/bin/gcc
CFLAGS = -Qn -w -Wall -Wextra -Wpedantic -O3 -std=c99 \
         -Wconversion -Wmissing-prototypes \
         -DPARAMS=$(PARAMS) $(EXTRA_CFLAGS) \
         -pthread -D_GNU_SOURCE \
         $(path) \





# 源文件路径重构
BASE_SRCDIR = src
SOURCES = $(BASE_SRCDIR)/signature/address.c \
          $(BASE_SRCDIR)/signature/merkle.c \
          $(BASE_SRCDIR)/signature/wots.c \
          $(BASE_SRCDIR)/signature/wotsx1.c \
          $(BASE_SRCDIR)/signature/fors.c \
          $(BASE_SRCDIR)/signature/sign.c \
          $(BASE_SRCDIR)/signature/TSS_sign.c \
          $(BASE_SRCDIR)/utils/utils.c \
          $(BASE_SRCDIR)/utils/utilsx1.c \
          $(BASE_SRCDIR)/utils/randombytes.c \
          $(BASE_SRCDIR)/hash/SM3.c \
          $(BASE_SRCDIR)/hash/hash_SM3.c \
          $(BASE_SRCDIR)/hash/thash_SM3_$(THASH).c \
          $(BASE_SRCDIR)/thread/ThreadSafeQueue.c \
          $(BASE_SRCDIR)/thread/communication.c \
          $(BASE_SRCDIR)/crypto/VSS.c \


HEADERS = include/params/params.h \
          include/signature/address.h \
          include/signature/fors.h \
          include/signature/merkle.h \
          include/signature/wots.h \
          include/signature/wotsx1.h \
          include/utils/utils.h \
          include/utils/utilsx1.h \
          include/utils/randombytes.h \
          include/hash/hash.h \
          include/hash/thash.h \
          include/hash/SM3.h \
          include/hash/SM3_offsets.h \
          include/thread/ThreadSafeQueue.h \
          include/thread/communication.h \
          include/crypto/VSS.h \
          include/api.h \
          include/TSS_api.h \


LDLIBS += -lpthread -lcrypto

DET_SOURCES = $(SOURCES:$(BASE_SRCDIR)/utils/randombytes.%=$(BASE_SRCDIR)/utils/rng.%)
DET_HEADERS = $(HEADERS:include/utils/randombytes.%=include/utils/rng.%)

# 测试目标配置
TESTS = test/fors \
        test/spx

BENCHMARK = test/benchmark/benchmark

TSS = test/TSS


# 构建规则
.PHONY: clean test benchmark

default: PQCgenKAT_sign

all: PQCgenKAT_sign tests benchmarks

tests: $(TESTS)

test: print_params $(TESTS:=.exec)

benchmarks: $(BENCHMARK)

benchmark: print_params $(BENCHMARK:=.exec)

tss: $(TSS)

TSS: $(TSS:=.exec)

# 主目标构建
PQCgenKAT_sign: test/KAT/PQCgenKAT_sign.c $(DET_SOURCES) $(DET_HEADERS)
	@ $(CC) $(CFLAGS) -o $@ $(DET_SOURCES) $< -lcrypto

# 性能测试构建
test/benchmark/benchmark: test/benchmark/benchmark.c \
                         test/benchmark/cycles.c \
                         $(SOURCES) $(HEADERS)
	@ $(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# 单元测试构建
test/%: test/%.c $(SOURCES) $(HEADERS)
	@ $(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

test/%.exec: test/%
	@$<

# TSS测试构建
test/TSS: test/TSS.c $(SOURCES) $(HEADERS)
	@ $(CC) $(CFLAGS) -o $@ $(SOURCES) $< $(LDLIBS)

clean:
	-$(RM) $(TESTS)
	-$(RM) $(BENCHMARK)
	-$(RM) $(TSS)
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