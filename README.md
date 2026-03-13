# Windonux - Linux-like Commands for Windows

## 📝 项目简介
Windonux是一个为Windows系统开发的Linux风格命令工具集，让你在Windows环境中也能使用熟悉的Linux命令行体验。

**⚠️ 测试版声明**
这是一个测试版本，部分功能可能还不完善，欢迎反馈和建议！

## ✨ 已实现的命令

### 核心文件操作
- `ls` - 列出目录内容
- `pwd` - 显示当前工作目录
- `mkdir` - 创建目录
- `rm` - 删除文件和目录
- `cp` - 复制文件和目录
- `mv` - 移动或重命名文件和目录
- `cat` - 显示文件内容
- `touch` - 创建文件或更新时间戳

### 系统命令
- `reboot` - 重启系统
- `shutdown` - 关闭系统
- `passwd` - 修改用户密码（开发中）
- `useradd` - 添加用户（开发中）
- `userdel` - 删除用户（开发中）

### 系统监控
- `ps` - 显示系统进程
- `df` - 显示磁盘空间使用情况
- `du` - 显示目录大小
- `whoami` - 显示当前用户名

### 文本处理
- `echo` - 显示文本
- `grep` - 搜索文本模式
- `sort` - 排序文本
- `head` - 显示文件开头
- `tail` - 显示文件结尾
- `wc` - 统计文件行数、单词数和字符数

### 实用工具
- `clear` - 清除终端屏幕
- `find` - 搜索文件和目录
- `which` - 查找命令位置（开发中）

## 🚀 快速开始

### 安装依赖
- **GCC编译器**：推荐使用MinGW或Cygwin
- **Node.js**（可选）：用于运行自动化编译脚本

### 编译方法

#### 使用Node.js自动化编译（推荐）
```bash
node build.js
```

#### 使用Windows批处理脚本
```bash
build.bat
```

### 安装使用
1. 将生成的`bin`目录添加到系统PATH环境变量
2. 在命令提示符或PowerShell中使用Linux风格命令

## 📖 使用示例

```bash
# 列出当前目录内容
ls -la

# 显示当前工作目录
pwd

# 创建目录
mkdir newdir

# 复制文件
cp file.txt newdir/

# 显示文件内容
cat file.txt

# 搜索文本
find . "*.c"
grep "pattern" file.txt

# 查看系统进程
ps

# 查看磁盘使用情况
df

# 统计文件行数
wc -l file.txt
```

## 📁 项目结构

```
windonux/
├── src/          # C语言源代码
├── bin/          # 编译后的可执行文件
├── build.js      # Node.js自动化编译脚本
├── build.bat     # Windows批处理脚本
├── .gitignore    # Git忽略文件
└── README.md     # 项目说明文档
```

## 🛠️ 开发计划

### 即将实现
- `chmod` - 修改文件权限
- `chown` - 修改文件所有者
- `sed` - 流编辑器
- `awk` - 文本处理工具

### 功能增强
- 完善正则表达式支持
- 添加更多命令行参数
- 提高命令执行效率

## 📄 许可证

本项目采用MIT许可证，详见LICENSE文件（待添加）。

## 🤝 贡献

欢迎提交Issue和Pull Request！

## 📞 联系方式

- GitHub: https://github.com/yxpil/windonux
- Email: yxpil@example.com

---

**版本：vESC201-2026313**
**发布日期：2026-03-13**
**状态：测试版**