# SM3实例化的SPHINCS+门限签名方案

## 项目简介
本项目是基于后量子密码标准SPHINCS+的门限签名系统实现，创新性地结合了**多线程模拟测试框架**和**实际网络通信实现**的双重签名-验证架构。

## 项目特点
- **基于 SM3**：使用国密算法 SM3 作为底层哈希实例化 SPHINCS+
- **SPHINCS+ 实现**：完整实现了 SPHINCS+ 签名算法，包括密钥生成、签名和验证功能。
- **高效且安全**：代码经过优化，兼顾性能和安全性。
- **分布式秘密共享**：独立实现分布式Shamir秘密共享
- **多线程模拟TSS签名环境**
   - 单进程模拟多方签名流程
   - 独立线程运行每个参与方
   - 内存队列实现线程间通信

- **真实网络通信实现TSS**
   - 基于TCP的分布式通信
   - 使用线程池实现并发处理网络连接，提高吞吐量
   - 支持跨主机部署

- **前端UI用于测试和使用**
  - 使用flask制作前端，实现基于网络的TSS方案
  - 前后端协同，灵活调整参数
  - 简洁UI降低使用门槛

## 依赖项
- **编译器**：GCC
- **构建工具**：Make
- **序列化工具**: CJson
- **Web框架**: Flask
- **HTTP 服务器**: Gunicorn (可选)

## ref方案构建与运行

### 1. 克隆项目
```bash
git clone https://github.com/Seallver/SphincsplusSM3.git
cd SphincsplusSM3/ref
```
### 2. 构建项目
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

## 门限签名方案构建

### 1. 构建并运行多线程模拟测试
```bash
cd SphincsplusSM3/TSS
make tss
```
### 2. 编译网络通信
```bash
make nets
```
### 3. 查看IP地址和门限签名参数，可修改，确保IP地址和端口准确无误
```bash
cd SphincsplusSM3/TSS/network
cat net_settings.h
```
### 4. 运行keygen
```bash
cd SphincsplusSM3/TSS/keygen
```
#### TTP
```bash
./TTP
```
#### 参与方
```bash
./client <party_id>
```

### 5. 运行sign
```bash
cd SphincsplusSM3/TSS/sign
```
#### TTP
```bash
./TTP
```
#### 门限参与方
```bash
./client <party_id>
```

### 6. 运行verify
```bash
cd SphincsplusSM3/TSS/verify
./verify <filename>
```



