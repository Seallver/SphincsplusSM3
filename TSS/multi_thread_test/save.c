#include <stdio.h>
#include <stdlib.h>

#define NUM_PARAMS 6
#define NUM_THASH 2

// PARAMS 和 THASH 列表
const char* PARAMS_LIST[NUM_PARAMS] = {
    "128s", "128f", "192s", "192f", "256s", "256f"
};

const char* THASH_LIST[NUM_THASH] = {
    "simple", "robust"
};

// 写入CSV文件的函数
void write_to_csv(const char* filename, const char* params, const char* thash, double total_elapsed) {
    FILE *file = fopen(filename, "a");  // 打开文件以追加模式
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // 根据 param 和 thash 的组合，确定位置并填充执行时间
    int param_idx = -1;
    int thash_idx = -1;

    // 查找 param 和 thash 的索引
    for (int i = 0; i < NUM_PARAMS; i++) {
        if (strcmp(PARAMS_LIST[i], params) == 0) {
            param_idx = i;
            break;
        }
    }

    for (int j = 0; j < NUM_THASH; j++) {
        if (strcmp(THASH_LIST[j], thash) == 0) {
            thash_idx = j;
            break;
        }
    }

    fprintf(file, "%s,%s,%f\n", params, thash, total_elapsed);
    printf("Wrote to CSV: %s, %s, %f\n", params, thash, total_elapsed);
    
    fclose(file);
}