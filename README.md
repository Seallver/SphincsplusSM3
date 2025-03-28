# SM3实例化的SPHINCS+门限签名方案

## 项目简介
本项目是基于国密算法 SM3 的 SPHINCS+ 后量子签名算法的实现。SPHINCS+ 是一种基于哈希函数的后量子签名方案，具有抗量子计算攻击的特性。本实现使用 SM3 作为底层哈希函数，适用于需要国密算法支持的场景。

## 项目特点
- **基于 SM3**：使用国密算法 SM3 作为哈希函数，符合中国密码算法标准。
- **SPHINCS+ 实现**：完整实现了 SPHINCS+ 签名算法，包括密钥生成、签名和验证功能。
- **高效且安全**：代码经过优化，兼顾性能和安全性。
- **门限签名实现**：进行门限签名的方案设计，并通过多线程模拟实现
## 依赖项
- **编译器**：GCC
- **构建工具**：Make

## 构建与运行

### 1. 克隆项目
```bash
git clone https://github.com/Seallver/SphincsplusSM3.git
cd SphincsplusSM3/ref
```
### 2. 编译项目
```bash
make
```
### 3. 运行测试
```bash
make test
```
### 4. 运行性能测试
```bash
make benchmark
```
### 5. KAT测试
```bash
./PQCgenKAT_sign
```
### 6. 清理构建文件
```bash
make clean
```
### 7. 更改参数配置与thash结构
在ref/Makefile中，通过更改PARAMS和THASH实现，更改后需要重新编译

## 门限签名
### 1. 进入门限签名方案文件夹
```bash
cd SphincsplusSM3/TSS
```
### 2. 编译并运行项目
```bash
make TSS
```

