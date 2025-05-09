PARAMS = sphincs-SM3-256s
THASH = simple

CC = /usr/bin/gcc
CFLAGS = -Wall -Wextra -Wpedantic -O3 -std=c99 \
         -Wconversion -Wmissing-prototypes \
         -DPARAMS=$(PARAMS) $(EXTRA_CFLAGS) \
         -Iinclude -Iinclude/hash -Iinclude/signature -Iinclude/params -Iinclude/utils  # 新增头文件路径

# 源文件路径重构
BASE_SRCDIR = src
SOURCES = $(BASE_SRCDIR)/signature/address.c \
          $(BASE_SRCDIR)/randombytes.c \
          $(BASE_SRCDIR)/signature/merkle.c \
          $(BASE_SRCDIR)/signature/wots.c \
          $(BASE_SRCDIR)/signature/wotsx1.c \
          $(BASE_SRCDIR)/utils/utils.c \
          $(BASE_SRCDIR)/utils/utilsx1.c \
          $(BASE_SRCDIR)/signature/fors.c \
          $(BASE_SRCDIR)/signature/sign.c

HEADERS = include/params/params.h \
          include/signature/address.h \
          include/randombytes.h \
          include/signature/merkle.h \
          include/signature/wots.h \
          include/signature/wotsx1.h \
          include/utils/utils.h \
          include/utils/utilsx1.h \
          include/signature/fors.h \
          include/api.h \
          include/hash/hash.h \
          include/hash/thash.h \
          
DET_SOURCES = $(SOURCES:$(BASE_SRCDIR)/randombytes.%=$(BASE_SRCDIR)/rng.%)
DET_HEADERS = $(HEADERS:include/randombytes.%=include/rng.%)

# SM3相关配置
ifneq (,$(findstring SM3,$(PARAMS)))
    SOURCES += $(BASE_SRCDIR)/hash/SM3.c \
               $(BASE_SRCDIR)/hash/hash_SM3.c \
               $(BASE_SRCDIR)/hash/thash_SM3_$(THASH).c
    HEADERS += include/hash/SM3.h \
               include/hash/SM3_offsets.h
endif

# 测试目标配置
TESTS = test/fors \
        test/spx

BENCHMARK = test/benchmark/benchmark

# 构建规则
.PHONY: clean test benchmark

default: PQCgenKAT_sign

all: PQCgenKAT_sign tests benchmarks

tests: $(TESTS)

test: print_params $(TESTS:=.exec)

benchmarks: $(BENCHMARK)

benchmark: print_params $(BENCHMARK:=.exec)

# 主目标构建
PQCgenKAT_sign: test/PQCgenKAT_sign.c $(DET_SOURCES) $(DET_HEADERS)
	$(CC) $(CFLAGS) -o $@ $(DET_SOURCES) $< -lcrypto

# 性能测试构建
test/benchmark/benchmark: test/benchmark/benchmark.c \
                         test/benchmark/cycles.c \
                         $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# 单元测试构建
test/%: test/%.c $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# 执行规则保持不变
test/%.exec: test/%
	@$<

clean:
	-$(RM) $(TESTS)
	-$(RM) $(BENCHMARK)
	-$(RM) PQCgenKAT_sign
	-$(RM) PQCsignKAT_*.rsp
	-$(RM) PQCsignKAT_*.req
	-$(RM) src/*.o src/*/*.o

print_params:
	@params_len=$$(echo -n "$(PARAMS)" | wc -c); \
	thash_len=$$(echo -n "$(THASH)" | wc -c); \
	border_len=$$((params_len + 19)); \
	printf "%0.s=" $$(seq 1 $$border_len); echo ""; \
	printf "||   PARAMS = %-$${params_len}s   ||\n" "$(PARAMS)"; \
	printf "||   THASH  = %-$${params_len}s   ||\n" "$(THASH)"; \
	printf "%0.s=" $$(seq 1 $$border_len); echo ""