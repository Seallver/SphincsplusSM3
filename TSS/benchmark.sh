#!/bin/bash

# 定义所有可能的 PARAMS 和 THASH 值
PARAMS_LIST=(
    "128s"
    "128f"
    "192s"
    "192f"
    "256s"
    "256f"
)

THASH_LIST=(
    "simple"
    "robust"
)


CSV_FILE="benchmark_results.csv"
echo -n "PARAMS," > "$CSV_FILE"
echo -n "THASH," >> "$CSV_FILE"
echo -n "RESULT" >> "$CSV_FILE"
echo "" >> "$CSV_FILE"

# 遍历所有组合
for PARAMS in "${PARAMS_LIST[@]}"; do
    for THASH in "${THASH_LIST[@]}"; do
        # 清理之前的构建
        make clean

        # 使用指定参数运行 make
        make tss PARAMS_HEAD="$PARAMS" THASH="$THASH" PRINT="NOTPRINT"

        # 检查是否构建成功
        if [ $? -eq 0 ]; then
            echo "✅ test successful for PARAMS=$PARAMS, THASH=$THASH"
        else
            echo "❌ test failed for PARAMS=$PARAMS, THASH=$THASH"
            exit 1  # 如果失败，终止脚本
        fi

        echo ""
    done
done

echo "🎉 All test completed successfully!"