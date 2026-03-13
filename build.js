const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

console.log('Linux-like Commands Compiler for Windows');
console.log('=======================================\n');

// 检查Node.js版本
const nodeVersion = process.versions.node;
const [major, minor] = nodeVersion.split('.').map(Number);
if (major < 12) {
    console.error('错误：需要Node.js 12或更高版本');
    process.exit(1);
}

// 创建输出目录
const binDir = path.join(__dirname, 'bin');
if (!fs.existsSync(binDir)) {
    fs.mkdirSync(binDir, { recursive: true });
    console.log(`创建输出目录: ${binDir}`);
}

// 检查是否有gcc编译器
let hasGcc = false;
try {
    execSync('gcc --version', { stdio: 'ignore' });
    hasGcc = true;
} catch (error) {
    // gcc不存在
}

if (!hasGcc) {
    console.error('错误：未找到gcc编译器！');
    console.error('请安装MinGW或其他GCC发行版并添加到系统PATH');
    process.exit(1);
}

// 获取src目录下的所有C文件
const srcDir = path.join(__dirname, 'src');
let cFiles;
try {
    cFiles = fs.readdirSync(srcDir)
        .filter(file => file.endsWith('.c'))
        .sort();
} catch (error) {
    console.error(`错误：无法读取src目录: ${error.message}`);
    process.exit(1);
}

if (cFiles.length === 0) {
    console.error('错误：src目录中没有找到C文件');
    process.exit(1);
}

console.log(`找到 ${cFiles.length} 个C文件需要编译:\n`);
cFiles.forEach(file => {
    console.log(`  - ${file}`);
});
console.log('');

// 编译每个C文件
let successCount = 0;
let errorCount = 0;

console.log('开始编译...\n');

cFiles.forEach(file => {
    const srcPath = path.join(srcDir, file);
    const exeName = path.basename(file, '.c') + '.exe';
    const binPath = path.join(binDir, exeName);
    
    let command;
    
    // 根据文件名添加特定的链接库
    switch (path.basename(file, '.c')) {
        case 'passwd':
            command = `gcc -o "${binPath}" "${srcPath}" -lnetapi32`;
            break;
        case 'cd':
            command = `gcc -o "${binPath}" "${srcPath}" -lshlwapi`;
            break;
        default:
            command = `gcc -o "${binPath}" "${srcPath}"`;
    }
    
    process.stdout.write(`编译 ${file}... `);
    
    try {
        execSync(command, { stdio: 'pipe' });
        console.log('✓ 成功');
        successCount++;
    } catch (error) {
        console.log('✗ 失败');
        console.error(`    错误信息: ${error.stderr.toString().trim()}`);
        errorCount++;
    }
});

console.log('\n' + '='.repeat(50));
console.log(`编译完成: ${successCount} 成功, ${errorCount} 失败`);

if (successCount > 0) {
    console.log('\n生成的可执行文件:');
    const exeFiles = fs.readdirSync(binDir)
        .filter(file => file.endsWith('.exe'))
        .sort();
    
    exeFiles.forEach(file => {
        console.log(`  - ${file}`);
    });
    
    console.log('\n使用方法:');
    console.log('1. 将 bin 目录添加到系统 PATH 环境变量');
    console.log('2. 在命令提示符或 PowerShell 中使用 Linux 风格命令');
    console.log('3. 例如: ls, cat, grep "pattern" file.txt');
}

console.log('='.repeat(50));

process.exit(errorCount > 0 ? 1 : 0);